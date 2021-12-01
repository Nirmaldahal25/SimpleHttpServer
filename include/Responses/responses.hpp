#ifndef RESPONSES 
#define RESPONSES 1

#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>


int sendData(int sockfd, const void *data, int data_size);
int sendData(int sockfd, const std::string &data);
int receiveData(int sockfd, void *data, int data_size);
int receiveData(int sockfd, std::string &data);

#endif