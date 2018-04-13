#ifndef SERVERSOCKET
#define SERVERSOCKET

#endif // SERVERSOCKET

#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/signal.h>
#include <cstring>
#include <memory>

#define MAX_CLIENT_BUFFER_SIZE 1024
#define SOCKET_ERROR -1

class ServerSocket {

private:
    std::string host, port;
    int socket;
    char buffer[MAX_CLIENT_BUFFER_SIZE];

    void initSocket();

public:
    ServerSocket() = delete;
    ~ServerSocket();

    ServerSocket(const std::string & host_, const std::string & port_);

    std::string read();
    void write(const std::string & buf);
    void close();


};

class Server {

private:
    ServerSocket socket;

public:
    Server() = delete;
    Server(const std::string & host, const std::string & port) :
        socket(host, port) { }
    ~Server() { socket.close(); }

    void close() { socket.close(); }

    std::string read() { return socket.read(); }
    void write(const std::string & buf) { socket.write(buf); }
};
