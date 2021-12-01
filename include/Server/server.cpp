#include "server.hpp"
sockaddr_in Server::setServerAddr(int port, const char *address)
{
    sockaddr_in server_address;
    server_address.sin_family = PF_INET;
    server_address.sin_port = htons(port);
    if (address == nullptr)
    {
        server_address.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        in_addr serveraddress;
        inet_aton(address, &serveraddress);
        // inet_addr();
        server_address.sin_addr.s_addr = serveraddress.s_addr;
    }
    return server_address;
}

int Server::init()
{
    int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //IPPROTO_TCP SOL_SOCKET
    if (server_socket == -1)
        std::cerr << "Socket inintialization error\n";
    return server_socket;
}

bool Server::binding(int sock, sockaddr_in& address)
{
    try{
        return bind(sock, (sockaddr *)&address, sizeof(address)) > 0;
    }
    catch(...)
    {
        return false;
    }
}

void Server::setSocketServerProperties(int sock)
{
    bool optval = true;
    int optlen = sizeof optval;
    if (setsockopt(sock, IPPROTO_TCP, SO_REUSEADDR, (char *)(&optval), (socklen_t)sizeof(optlen)) == -1)
        std::cerr << "SetsockOpt(): method instantiation failed\n";
}

void Server::listeners(int sockfd, int no)
{
    if(listen(sockfd, no)==-1) std::cerr<<"Listen() on socket faild\n";
}

void Server::closeServerSocket(int sockfd)
{
    close(sockfd);
}