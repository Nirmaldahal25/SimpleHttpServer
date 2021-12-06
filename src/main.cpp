#include "server.hpp"
#include "threadpool.hpp"
#include "argparse.hpp"
#include "files.hpp"

#include <iostream>
int main(int argc, char * argv[])
{
    Argparse<3> parse;
    parse.setArgument("--thread", "No of threads to use",DATA_TYPE::INTEGER);
    parse.setArgument("--ip", "Ip address to use", DATA_TYPE::STRING);
    parse.setArgument("--port", "Port No to listen on", DATA_TYPE::INTEGER);
    parse.parseArgument(argc-1, argv);

    int threadpoll = static_cast<int>(parse.getArg<int>(0).value_or(4));
    int port = static_cast<int>(parse.getArg<int>(2).value_or(80));
    std::string ip = static_cast<std::string>(parse.getArg<std::string>(1).value_or("127.0.0.1"));

    SocketDescriptor server; 
    server.sockfd = Server::init();
    Server::setSocketServerProperties(server.sockfd);
    server.address = Server::setServerAddr(port, ip.c_str());
    Server::listeners(server.sockfd, 10);

    Threadpool::setServer(server);
    Threadpool* pool = Threadpool::getInstance(threadpoll);
    while(pool->workState){
        pool->acceptConnections();
    }
    return 0;
}