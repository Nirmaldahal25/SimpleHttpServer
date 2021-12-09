#pragma once
#include <thread>
#include <list>
#include <utility>
#include <fcntl.h>

#include "server.hpp"
#include "threadpool.hpp"
#include "responses.hpp"
#include "Files.hpp"
#include "httprequests.hpp"

class Client
{
public:
    Client()=delete;
    Client(SocketDescriptor server,int);
    void setClientSockaddr(int client,sockaddr_in client_add);
    void setClientSocketOptions()noexcept;
    bool setBlockingMode(bool block) noexcept;
    static std::mutex climutex;
    static std::list<int> clientThreadReady;

    //On blocking Mode OS stops waiting for the request, default = 15 sec
    void blockingTime(int time = 15);
private:
    int clientid;
    SocketDescriptor client;

    SocketDescriptor server;
    
    void runevents()noexcept;
    const std::string getHostnameOfClient()noexcept;
};
