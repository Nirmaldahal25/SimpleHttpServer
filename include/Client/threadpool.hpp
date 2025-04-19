#ifndef THREADPOOL
#define THREADPOOL 1

#include <mutex>
#include <queue>
#include <map>
#include <netdb.h>
#include <functional>
#include <iostream>
#include <condition_variable>
#include <signal.h>
#include <system_error>
#include <atomic>

#include "server.hpp"
#include "client.hpp"
class Client;
class Threadpool
{
public:
    void enqueue(std::unique_ptr<Client> _client);

    bool isWorking() noexcept;

    Threadpool(const Threadpool &) = delete;
    void operator=(const Threadpool &) = delete;

    static Threadpool *getInstance(int pool);

    // Creates a copy of server instance and share between clients
    static void setServer(const SocketDescriptor &_server);

    // Start running the threadpool
    void run();

    // Accept connections from client
    void acceptConnections();

    ~Threadpool()
    {
        delete threadp;
    }

private:
    Threadpool(int) noexcept;

    // Create a unique lock
    std::unique_ptr<std::mutex> lock;

    // A condition variable to notify threads for execution
    std::condition_variable callwait;

    // Stopper to close all threads
    std::atomic<bool> stopper = false;

    std::queue<std::unique_ptr<Client>> tasks;

    static Threadpool *threadp;

    // A pointer that can be shared between clients
    std::shared_ptr<SocketDescriptor> server;
    std::vector<std::thread> clients_handler;

    // Create a threadpool of thread_count threads
    unsigned int threads_count;
    void handleConnections();

    // Signal Handler to SIGINT
    static void signalHandler(int);

    // Stop the threadpool.
    void stop();
};

#endif