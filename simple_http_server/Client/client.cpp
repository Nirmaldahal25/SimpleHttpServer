#include "client.hpp"

Client::Client(std::shared_ptr<SocketDescriptor> server, int client_fd, sockaddr_in client_addr) : m_server(server), m_client()
{
    m_client.address = client_addr;
    m_client.sockfd = client_fd;
}

bool Client::setBlockingMode(bool block) noexcept
{
    int flags = fcntl(m_client.sockfd, F_GETFL, 0);
    flags = block ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return (fcntl(m_client.sockfd, F_SETFL, flags) == 0) ? true : false;
}

void Client::blockingTime(int time)
{
    struct timeval tv = {
        .tv_sec = time,
    };
    setsockopt(m_client.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
}

const std::string Client::getHostnameOfClient() noexcept
{
    hostent *host;
    in_addr addr;
    std::string str;
    int i = 0;
    host = gethostbyaddr((char *)&(m_client.address.sin_addr.s_addr), sizeof(in_addr), PF_INET);
    if (host->h_name)
    {
        while (host->h_addr_list[i] != 0)
            addr.s_addr = *(u_long *)host->h_addr_list[i++];
        str = inet_ntoa(addr);
    }
    return str;
}

void Client::setClientSocketOptions() noexcept
{
    int a = 1;
    setsockopt(m_client.sockfd, SO_LINGER, IPPROTO_TCP, (void *)(&a), 4);
}

void Client::runevents() noexcept
{
    HTTPResponse response;
    std::string requestparse;

    std::string fileData;
    blockingTime(5);
    setClientSocketOptions();

    // Unoptimized code here
    while (true)
    {
        int received = Responses::recvResponse(m_client.sockfd, requestparse);
        std::cout << requestparse << std::endl;
        if (received == 0)
        {
            setBlockingMode(false);
            break;
        }
        else
        {
            auto returntype = RequestParser::requestType(requestparse);
            if (std::get<float>(returntype) != 1.1f)
            {
                response.setStatusCode(Status_Codes::HTTP_VERSION_NOT_SUPPORTED_505);
                response.setConnection(Connection::CLOSE);
                if (Responses::sendResponse(m_client.sockfd, response.getResponse()) == 0)
                    std::cerr << "Msg not sent 505\n";
                break;
            }
            // std::cout<<std::get<float>(returntype)<<std::endl;
            auto path = std::get<std::string>(returntype);
            if (path == "/")
            {
                response.setStatusCode(Status_Codes::OK_200);
                response.setConnection(Connection::CLOSE);
                fileData = Files::getDirectoryView("./");
                response.setContentType();
                response.setContentLength(fileData.size());
                Responses::sendResponse(m_client.sockfd, response.getResponse());
                Responses::sendResponse(m_client.sockfd, fileData);
                break;
            }
            auto path_exists = Files::checkForFileorFolder(path.substr(1));

            if (std::get<bool>(path_exists))
            {
                response.setStatusCode(Status_Codes::OK_200);
                if (RequestParser::getConnection(requestparse) == Connection::KEEP_ALIVE)
                {
                    response.setConnection(Connection::KEEP_ALIVE);
                    setBlockingMode(true);
                    blockingTime(10);
                }
                else
                    response.setConnection();

                if (std::get<Directory::Type>(path_exists) == Directory::Type::Folder)
                {
                    fileData = Files::getDirectoryView(path.substr(1));
                    response.setContentType();
                    response.setContentLength(fileData.size());
                }
                else
                {
                    // std::cout<<path<<std::endl;
                    if (!Files::readFromFile(path.substr(1), fileData, 0))
                        break;
                    response.setContentType();
                    response.setContentLength(fileData.size());
                }
                // std::cout<<response.getResponse()<<std::endl;
                // std::cout<<fileData<<std::endl;
                // auto Re = response.getResponse();
                Responses::sendResponse(m_client.sockfd, response.getResponse());
                if (fileData.size() != Responses::sendResponse(m_client.sockfd, fileData))
                {
                    std::cerr << "Couldn't send\n";
                }
                fileData.clear();
                continue;
            }
            else
            {
                response.setStatusCode(Status_Codes::NOT_FOUND_404);
                response.setConnection(Connection::CLOSE);
                Responses::sendResponse(m_client.sockfd, response.getResponse());
                break;
            }
        }
    }

    shutdown(m_client.sockfd, SHUT_RDWR);
    close(m_client.sockfd);
}
