#include "Server/server.hpp"
#include "Client/threadpool.hpp"
#include "Argparser/argparse.hpp"
#include <future>

int main(int argc, char *argv[])
{
    Argparse<3> parse;
    parse.setArgument("--thread", "No of threads to use", DATA_TYPE::INTEGER);
    parse.setArgument("--ip", "Ip address to use", DATA_TYPE::STRING);
    parse.setArgument("--port", "Port No to listen on", DATA_TYPE::INTEGER);
    parse.parseArgument(argc - 1, argv);

    int threadpollcount = static_cast<int>(parse.getArg<int>(0).value_or(5));
    int port = static_cast<int>(parse.getArg<int>(2).value_or(8000));
    std::string ip = static_cast<std::string>(parse.getArg<std::string>(1).value_or("127.0.0.1"));
    std::cout << "Ip: " << ip << " Port: " << port << std::endl;

    SocketDescriptor server;
    server.sockfd = Server::init();

    Server::setSocketServerProperties(server.sockfd);
    server.address = Server::setServerAddr(port, nullptr);

    Server::binding(server);
    Server::listeners(server.sockfd, 5);

    Threadpool *threadpool = Threadpool::getInstance(threadpollcount);
    threadpool->setServer(server);
    threadpool->run();
    while (threadpool->isWorking())
    {
        threadpool->acceptConnections();
    }

    return 0;
}