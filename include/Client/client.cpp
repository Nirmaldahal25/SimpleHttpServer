#include "client.hpp"

Client::Client(SocketDescriptor _server, int id):clientid(id)
{
    this->server.sockfd = _server.sockfd;
    this->server.address = _server.address;
    Client::clientThreadReady.push_back(id);
}

bool Client::setBlockingMode(bool block) noexcept
{
    int flags = fcntl(client.sockfd, F_GETFL, 0);
    flags = block ? (flags & ~O_NONBLOCK):(flags | O_NONBLOCK);
    return (fcntl(client.sockfd, F_SETFL, flags) == 0) ? true : false;
}

void Client::setClientSockaddr(int client, sockaddr_in client_add)
{
    this->client.address = client_add;
    this->client.sockfd= client;
    runevents();
}

void Client::blockingTime(int time)
{
    struct timeval tv = {
        .tv_sec = time,
    };
    setsockopt(this->client.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
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
    HTTPResponse response;
    std::string requestparse;
    
    std::string fileData;
    blockingTime(5);
    setClientSocketOptions();

    while(true){
        requestparse.clear();
        int received = Responses::recvResponse(client.sockfd, requestparse);
        std::cout<<requestparse<<std::endl;
        if(received == 0) 
        {
            setBlockingMode(false);
            break;
        }
        else
        {
            auto returntype = RequestParser::requestType(requestparse);
            if(std::get<float>(returntype) != 1.1f)
            {   
                response.setStatusCode(Status_Codes::HTTP_VERSION_NOT_SUPPORTED_505);
                response.setConnection(Connection::CLOSE);
                if(Responses::sendResponse(client.sockfd, response.getResponse())== 0) std::cerr<<"Msg not sent 505\n";
                break;
            }
            // std::cout<<std::get<float>(returntype)<<std::endl;
            auto path = std::get<std::string>(returntype);
            if(path == "/")
            {
                response.setStatusCode(Status_Codes::OK_200);
                response.setConnection(Connection::CLOSE);
                fileData = Files::getDirectoryView("./");
                response.setContentType();
                response.setContentLength(fileData.size());
                Responses::sendResponse(client.sockfd, response.getResponse());
                Responses::sendResponse(client.sockfd, fileData);
                break;
            }
            auto path_exists = Files::checkForFileorFolder(path.substr(1));

            if(std::get<bool>(path_exists))
            {   
                response.setStatusCode(Status_Codes::OK_200);
                if(RequestParser::getConnection(requestparse) == Connection::KEEP_ALIVE)
                {
                    response.setConnection(Connection::KEEP_ALIVE);
                    setBlockingMode(true);
                    blockingTime(10);
                }
                else response.setConnection();

                if(std::get<Directory::Type>(path_exists) == Directory::Type::Folder)
                {
                    fileData = Files::getDirectoryView(path.substr(1));
                    response.setContentType();
                    response.setContentLength(fileData.size());
                }
                else
                {
                    // std::cout<<path<<std::endl;
                    if(!Files::readFromFile(path.substr(1),fileData , 0)) break;
                    response.setContentType();
                    response.setContentLength(fileData.size());
                }
                // std::cout<<response.getResponse()<<std::endl;
                // std::cout<<fileData<<std::endl;
                // auto Re = response.getResponse();
                Responses::sendResponse(client.sockfd,response.getResponse());
                if(fileData.size() != Responses::sendResponse(client.sockfd, fileData))
                {
                    std::cerr<<"Couldn't send\n";
                }
                fileData.clear();
                continue;
            }
            else
            {
                response.setStatusCode(Status_Codes::NOT_FOUND_404);
                response.setConnection(Connection::CLOSE);
                Responses::sendResponse(client.sockfd, response.getResponse());
                break;
            }
        }
    }
    
    shutdown(client.sockfd,SHUT_RDWR);
    close(client.sockfd);
    {   
        std::lock_guard<std::mutex> lock(Client::climutex);
        Client::clientThreadReady.push_back(this->clientid);
    }
}
