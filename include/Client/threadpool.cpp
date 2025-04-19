#include "threadpool.hpp"
Threadpool::Threadpool(int pool) noexcept : lock(new std::mutex()), server(nullptr), threads_count(pool)
{
}

Threadpool *Threadpool::threadp = nullptr;

Threadpool *Threadpool::getInstance(int pool)
{
    if (Threadpool::threadp == nullptr)
    {
        Threadpool::threadp = new Threadpool(pool);
        signal(SIGINT, Threadpool::signalHandler);
    }
    return Threadpool::threadp;
}

void Threadpool::signalHandler(int signum)
{
    std::cout << "\nSoft Shutdown..." << std::endl;
    threadp->stop();
}

void Threadpool::handleConnections()
{
    for (int i = 0; i < threads_count; i++)
    {
        clients_handler.emplace_back([this, i]()
                                     {
                while(true){
                    {
                        std::unique_lock lock(*this->lock);
                        callwait.wait(lock,[&]{ return stopper || !tasks.empty(); }); 
                        if(stopper || tasks.empty()){
                            break;
                        }
                        std::unique_ptr<Client> client=std::move(tasks.front());
                        tasks.pop();
                        client->runevents();
                    }
                } });
    }
}

void Threadpool::enqueue(std::unique_ptr<Client> _client)
{
    std::lock_guard guard(*this->lock);
    tasks.emplace(std::move(_client));
    callwait.notify_one();
}

void Threadpool::stop()
{
    stopper = true;
    callwait.notify_all();
    for (auto &thread : clients_handler)
    {
        thread.join();
    }
    Threadpool::clients_handler.clear();

    // shutdown server socket
    shutdown(server->sockfd, SHUT_RDWR);
    close(server->sockfd);
}

void Threadpool::acceptConnections()
{
    int client_socket;
    sockaddr_in client_addr;
    int length = sizeof(client_addr);

    client_socket = accept(server->sockfd, (sockaddr *)(&client_addr), (socklen_t *)(&length));
    if (client_socket != -1)
    {
        enqueue(std::make_unique<Client>(server, client_socket, client_addr));
    }

    if (client_socket == -1 && !stopper)
    {
        perror("Error accepting connections");
    }
}

bool Threadpool::isWorking() noexcept
{
    return !stopper;
}

void Threadpool::setServer(const SocketDescriptor &_server)
{
    threadp->server.reset(new SocketDescriptor(_server));
}

void Threadpool::run()
{
    if (server == nullptr)
    {
        throw "The server is not set yet";
    }
    threadp->handleConnections();
}