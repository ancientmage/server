#ifndef SERVERSOCKET
#define SERVERSOCKET

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <memory>
#include <ws2tcpip.h>
#include <WinSock2.h>

#define MAX_CLIENT_BUFFER_SIZE 1024

class ServerSocket {
private:
    std::string host;
    unsigned short port;
    int socket;

public:
    ServerSocket() = delete;
    ~ServerSocket();

    ServerSocket(int socket);

    std::string read() const;
    void write(const std::string & buf) const;
    void close();
};

class Server {

public:
    static void connection(const ServerSocket * socket) 
    {
        socket->write("hello from server\n");
        std::string s = socket->read();
        std::clog << "get from client : " << s << std::endl;
        delete socket;
    }

    Server() = delete;

    Server(const std::string & host, unsigned short port)
    {
        auto wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        WSAStartup(wVersionRequested, &wsaData);

        int listen_socket = ::socket(AF_INET, SOCK_STREAM, 0);
        BOOL opt = TRUE;
        setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        struct sockaddr_in address { 0 };
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(host.c_str());
        address.sin_port = htons(port);

        int result = bind(listen_socket, (struct sockaddr*)&address, sizeof(address));

        if (result == SOCKET_ERROR)
            throw std::runtime_error("bind failed");

        if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
            throw std::runtime_error("listen failed");
        
        while (true) {
            int socket = accept(listen_socket, NULL, NULL);
            ServerSocket * sock = new ServerSocket(socket);
            std::thread th(connection, sock);
            th.detach();
        }
        printf("socket failed with error: %ld\n", WSAGetLastError());
        
        return;
    }
    ~Server() { close(); }

    void close() { WSACleanup(); }
};


#endif // SERVERSOCKET