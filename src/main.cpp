#include "server.hpp"
#include "client.hpp"
#include "threadpool.hpp"

int main()
{
    SocketDescriptor server; 
    server.sockfd = Server::init();
    Server::setSocketServerProperties(server.sockfd);
    server.address = Server::setServerAddr(80, "127.0.0.1");
    Server::listeners(server.sockfd, 10);

    Threadpool::setServer(server);
    Threadpool* pool = Threadpool::getInstance(4);
    while(pool->workState){
        pool->acceptConnections();
    }
    return 0;
}