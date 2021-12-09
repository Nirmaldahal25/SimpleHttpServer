#include "server.hpp"
sockaddr_in Server::setServerAddr(int port, const char *address)
{
    sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;
    if (address == nullptr)
    {
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        server_address.sin_addr.s_addr = inet_addr(address);
    }
    return server_address;
}

int Server::init()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); //IPPROTO_TCP SOL_SOCKET
    if (server_socket == -1)
        perror( "Socket inintialization error\n");
    return server_socket;
}

void Server::binding(SocketDescriptor& server)
{   
    int rc =  bind(server.sockfd,(struct sockaddr *)&(server.address),sizeof(server.address));
    if( rc == -1)
    {
        perror("binding failed");
    }
}

void Server::setSocketServerProperties(int sock)
{
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        perror("SetsockOpt(): SO_REUSEADDR failed\n");
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == -1)
        perror("SetsockOpt(): SO_REUSEADDR failed\n");
}

void Server::listeners(int sockfd, int no)
{
    if(listen(sockfd, no)==-1) perror("Listen() on socket faild\n");
}

void Server::closeServerSocket(int sockfd)
{
    close(sockfd);
}