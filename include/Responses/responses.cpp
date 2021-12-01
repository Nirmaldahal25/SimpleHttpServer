#include "responses.hpp"

int sendData(int sockfd, const void *data, int data_size)
{
    const char *data_ptr = (const char*) data;
    int bytes_sent;

    while (data_size > 0)
    {
        bytes_sent = send(sockfd, data_ptr, data_size, 0);
        if (bytes_sent == -1)
            return -1;

        data_ptr += bytes_sent;
        data_size -= bytes_sent;
    }

    return 1;
}

int sendData(int sockfd, const std::string &data)
{
    ulong data_size = htonl(data.size());

    int result = sendData(sockfd, &data_size, sizeof(data_size));
    if (result == 1)
        result = sendData(sockfd, data.c_str(), data.size());

    return result;
}

int receiveData(int sockfd, void *data, int data_size)
{
    char *data_ptr = (char*) data;
    int bytes_recv;

    while (data_size > 0)
    {
        bytes_recv = recv(sockfd, data_ptr, data_size, 0);
        if (bytes_recv <= 0)
            return bytes_recv;

        data_ptr += bytes_recv;
        data_size -= bytes_recv;
    }

    return 1;
}

int receiveData(int sockfd, std::string &data)
{
    ulong data_size;
    int result;

    data = "";

    result = receiveData(sockfd, &data_size, sizeof(data_size));
    if (result == 1)
    {
        data_size = ntohl(data_size);
        if (data_size > 0)
        {
            data.resize(data_size);

            result = receiveData(sockfd, &data[0], data_size);
            if (result != 1)
                data.clear();
        }
    }

    return result;
}
