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
    Client() = delete;
    Client(std::shared_ptr<SocketDescriptor> server, int client_fd, sockaddr_in client_addr);
    void setClientSocketOptions() noexcept;
    bool setBlockingMode(bool block) noexcept;

    // On blocking Mode OS stops waiting for the request, default = 15 sec
    void blockingTime(int time = 15);

    // runevents
    void runevents() noexcept;

private:
    SocketDescriptor m_client;

    std::weak_ptr<SocketDescriptor> m_server;

    const std::string getHostnameOfClient() noexcept;
};
