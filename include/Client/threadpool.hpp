#ifndef THREADPOOL
#define THREADPOOL 1

#include <mutex>
#include <queue>
#include <map>
#include <netdb.h>
#include <functional>
#include <iostream>
#include <condition_variable>
#include <signal.h>

#include "server.hpp"
#include "client.hpp"
class Client;
class Threadpool
{
public:
    using Tasks=std::function<void()>;
    void enqueue(Tasks task);
    std::queue<Tasks> tasks;
    std::mutex locker;
    std::mutex climutex;
    std::condition_variable callwait;
    bool workState = true;

    Threadpool(const Threadpool&) = delete;
    void operator=(const Threadpool&) = delete;
    
    static Threadpool* getInstance(int pool = 1);
    static void setServer(const SocketDescriptor& _server);

    void acceptConnections();

    ~Threadpool(){
        delete threadp;
    }   
private:
    Threadpool(int)noexcept;


    bool stopper = false;
    bool stopped = false;
    
    static Threadpool* threadp;
    static SocketDescriptor server;

    std::vector<std::thread> clients_handler;
    std::vector<Client> clients;

    void handleConnections(int pool);
    static void signalHandler(int);
    void stop();
};

#endif