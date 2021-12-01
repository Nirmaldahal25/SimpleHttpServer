#include "threadpool.hpp"
SocketDescriptor Threadpool::server;
Threadpool::Threadpool(int pool) noexcept
{
    for(int i=0;i<pool;i++)
        clients.emplace_back(Client(server, i));
    this->handleConnections(pool);
}

Threadpool* Threadpool::threadp = nullptr;

void Threadpool::setServer(const SocketDescriptor& _server)
{
    Threadpool::server = _server;
}

Threadpool* Threadpool::getInstance(int pool)
{
    if(Threadpool::threadp == nullptr){
        Threadpool::threadp = new Threadpool(pool);
        signal(SIGINT, Threadpool::signalHandler);
    }
    return Threadpool::threadp;
}

void Threadpool::signalHandler(int signum)
{
    Threadpool::getInstance()->workState = false;
    Threadpool::getInstance()->stop();
}

void Threadpool::handleConnections(int pool)
{   
    for(int i=0;i<pool;i++){
        clients_handler.emplace_back([this]{
            while(true){
                Tasks task;
                {
                    std::unique_lock lock(locker);
                    callwait.wait(lock,[=]{
                        return stopper || !tasks.empty(); });
                    task=std::move(tasks.front());
                    tasks.pop();
                    if(stopper)
                    {
                        if(!stopped) this->stop();  
                        break;
                    }
                }
                task();
            }
        }
        );
    }
}

void Threadpool::enqueue(Tasks task)
{
    {
        std::lock_guard guard(locker);
        tasks.emplace(std::move(task));
    }
    callwait.notify_one();
}

void Threadpool::stop()
{   
    this->workState = false;
    {
        std::unique_lock<std::mutex> lock(locker);
        stopper = true;
        stopped = true;
    }
    callwait.notify_all();
    for(auto& thread:clients_handler)
        thread.join();
    shutdown(server.sockfd, SHUT_RDWR);
    Threadpool::clients_handler.clear();  
}

void Threadpool::acceptConnections()
{
    int client_socket;
    sockaddr_in client_addr;
    int length = sizeof(client_addr);
    client_socket = accept(server.sockfd, (sockaddr *)(&client_addr), (socklen_t *)(&length));
    if (client_socket == -1) return;

    {
        std::lock_guard<std::mutex> lock(Client::climutex);
        if(Client::clientThreadReady.size()!=0)
        {
            enqueue(std::bind(&Client::setClientSockaddr,
            &clients[Client::clientThreadReady.front()],client_socket,client_addr));
            Client::clientThreadReady.pop_front();
        }
    }
}

