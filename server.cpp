#include "server.h"

ServerSocket::ServerSocket(const std::string &host_, const std::string &port_) :
    host(host_), port(port_)
{
    initSocket();
}

void ServerSocket::initSocket()
{
    struct addrinfo* addr = NULL;
    struct addrinfo hints;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addr);

    if (result != 0)
    {
        throw std::runtime_error("getaddrinfo failed");
        return;
    }

    int listen_socket = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

    if (listen_socket == SOCKET_ERROR)
    {
        freeaddrinfo(addr);
        throw std::runtime_error("error at creating socket");
    }

    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR)
    {
        freeaddrinfo(addr);
        throw std::runtime_error("bind failed");
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        freeaddrinfo(addr);
        throw std::runtime_error("listen failed");
    }

    socket = accept(listen_socket, NULL, NULL);
    if (socket == SOCKET_ERROR)
    {
        freeaddrinfo(addr);
        throw std::runtime_error("accept failed");
    }

    return;
}

ServerSocket::~ServerSocket()
{
    close();
}

std::string ServerSocket::read()
{
    if (socket == SOCKET_ERROR)
        throw std::runtime_error("cant read buffer because of socket isnt initialized");

    int result = recv(socket, buffer, MAX_CLIENT_BUFFER_SIZE, 0);
    if (result == SOCKET_ERROR)
        throw std::runtime_error("read failed");

    return std::string(buffer, result);
}

void ServerSocket::close() {
    ::close(socket);
}

void ServerSocket::write(const std::string & buffer) {
    if (socket == SOCKET_ERROR)
        throw std::runtime_error("cant write buffer because of socket isnt initialized");

    int result = send(socket, buffer.c_str(), buffer.size() + 1, 0);

    if (result == SOCKET_ERROR)
        throw std::runtime_error("send failed");
}

