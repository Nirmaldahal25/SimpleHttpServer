#include "server.hpp"
#include "threadpool.hpp"
#include "argparse.hpp"

int main(int argc, char * argv[])
{
    Argparse<3> parse;
    parse.setArgument("--thread", "No of threads to use",DATA_TYPE::INTEGER);
    parse.setArgument("--ip", "Ip address to use", DATA_TYPE::STRING);
    parse.setArgument("--port", "Port No to listen on", DATA_TYPE::INTEGER);
    parse.parseArgument(argc-1, argv);

    int threadpoll = std::get<int>(parse.getArg(0));
    int port = std::get<int>(parse.getArg(2));

    SocketDescriptor server; 
    server.sockfd = Server::init();
    Server::setSocketServerProperties(server.sockfd);
    server.address = Server::setServerAddr(80, "127.0.0.1");
    Server::listeners(server.sockfd, 10);

    Threadpool::setServer(server);
    Threadpool* pool = Threadpool::getInstance(threadpoll);
    while(pool->workState){
        pool->acceptConnections();
    }
    return 0;
}