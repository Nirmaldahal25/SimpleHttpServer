#include "client.hpp"

std::string obtainPath(std::string& request)
{
    std::string head;
    int k = -1;
    head.reserve(25);
    for (int i = 0; i < request.size(); i++)
    {
        if (request[i] == ' ')
        {
            k = -k;
            if (k < 0)
                break;
            continue;
        }
        if (k > 0)
            head += request[i];
    }
    return head;
}

Client::Client(SocketDescriptor _server, int id): servers{std::move(_server)}, clientid(id)
{
    Client::clientThreadReady.push_back(id);
}

bool Client::setBlockingMode(int sockfd, bool block) noexcept
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags = block ? (flags & ~O_NONBLOCK):(flags | O_NONBLOCK);
    return fcntl(sockfd, F_SETFL, flags) ? true : false;
}

void Client::setClientSockaddr(int client, sockaddr_in client_add)noexcept
{
    this->client.address = client_add;
    this->client.sockfd= client;
    runevents();
}

std::mutex Client::climutex;
std::list<int> Client::clientThreadReady;

const std::string Client::getHostnameOfClient()noexcept
{
    hostent *host;
    in_addr addr;
    std::string str;
    int i = 0;
    host = gethostbyaddr((char *)&(client.address.sin_addr.s_addr), sizeof(in_addr), PF_INET);
    if (host->h_name)
    {
        while (host->h_addr_list[i] != 0)
            addr.s_addr = *(u_long *)host->h_addr_list[i++];
        str = inet_ntoa(addr);
    }
    return str;
}

void Client::setClientSocketOptions()noexcept{
    int a=1;
    setsockopt(this->client.sockfd,SO_LINGER,IPPROTO_TCP,(void*)(&a),4);
}

void Client::runevents()noexcept
{
    std::string receivedHeaders;
    // Files file;
    std::string filedata, resp;
    receivedHeaders.reserve(512);
    setClientSocketOptions();
    {
        //response and requests atarts from here
        // getRequests(client_socket,receivedHeaders);
        std::cout<<getHostnameOfClient()<<'\n'<<receivedHeaders<<'\n';
        if(receivedHeaders.size()==0)
        { 
            std::cerr<<"Socket Closed: UnReceived Requests"<<"\n";
            // sendResponse(client_socket, std::string("HTTP/1.1 404 \r\n\r\n"));
            close(client.sockfd); // if no requests is sent then close client socket
        }
        else
        {
            auto path = obtainPath(receivedHeaders);
            if(path.find(".html")!=std::string::npos)
            {
                resp = "HTTP/1.1 200 \r\n"
                "Content-Type : text/html \r\n"
                "Content-Length : ";
                // file.readFileToBuffer(path, Filetype::template_files, 1);
            }
            else if(path.find("favicon")!=std::string::npos)
            {
                 resp = "HTTP/1.1 200 \r\n"
                "Content-Type : text/css, text/js, image/png \r\n"
                "Content-Length : ";
                path = "static/hel.png";
                // file.readFileToBuffer(path, Filetype::static_files, 0);
            }
            else
            {
                resp = "HTTP/1.1 200 \r\n"
                "Content-Type : text/css, text/js, image/png \r\n"
                "Content-Length : ";
                path.erase(0,1);
                // file.readFileToBuffer(path, Filetype::static_files, 0);
            }
            // file.getBufferData(filedata);
            resp += std::to_string(filedata.size()) + " \r\n\r\n";
            // sendResponse(client_socket, resp);
            // sendResponse(client_socket, filedata);
        }
    }
    shutdown(client.sockfd,SHUT_RDWR);
    close(client.sockfd);
    {   
        std::lock_guard<std::mutex> lock(Client::climutex);
        Client::clientThreadReady.push_back(this->clientid);
    }
}
