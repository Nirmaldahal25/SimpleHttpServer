#pragma once
#include <sys/socket.h>
#include <sys/unistd.h> //close(socket)
#include <netinet/in.h> // sockaddr_in structure
#include <iostream>
#include <arpa/inet.h>
// #include <string.h> //memset and memcopy
// #include <sys/types.h>
// #include <sys/un.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <string.h>
// #include <arpa/nameser.h>

struct SocketDescriptor
{
    int sockfd;
    sockaddr_in address;
};
namespace Server{
    int init();
    sockaddr_in setServerAddr(int port = 80, const char *ipaddress = nullptr);
    void setSocketServerProperties(int sock);
    bool binding(int sock, sockaddr_in& address);
    void listeners(int sock, int no);
    void closeServerSocket(int sockfd);
};
