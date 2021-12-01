#pragma once
#include <thread>
#include <list>
#include <utility>
#include <fcntl.h>

#include "threadpool.hpp"
#include "Files.hpp"

class Client
{
public:
    Client()=delete;
    Client(SocketDescriptor server,int);
    void setClientSockaddr(int client,sockaddr_in client_add)noexcept;
    void setClientSocketOptions()noexcept;
    bool setBlockingMode(int sockfd, bool block) noexcept;
    static std::mutex climutex;
    static std::list<int> clientThreadReady;
private:
    int clientid;
    SocketDescriptor client;

    std::vector<SocketDescriptor> servers;
    
    void runevents()noexcept;
    const std::string getHostnameOfClient()noexcept;
};
