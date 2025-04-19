#ifndef RESPONSES
#define RESPONSES 1

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

namespace Responses
{
    // send in Network Byte Order
    int sendData(int sockfd, const std::string &data);

    // receive in Network Byte Order
    int receiveData(int sockfd, std::string &data);

    int sendData(int sockfd, const void *data, int data_size);
    int receiveData(int sockfd, void *data, int data_size);

    // recv data from sockfd
    int recvResponse(int sockfd, std::string &str, int size = 5000);

    // send data to sockfd
    int sendResponse(int sockfd, const std::string &str);
};
#endif