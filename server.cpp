#include "server.h"

ServerSocket::ServerSocket(int socket)
{
    this->socket = socket;
}

ServerSocket::~ServerSocket()
{
    close();
}

std::string ServerSocket::read() const
{
    if (socket == SOCKET_ERROR)
        throw std::runtime_error("cant read buffer because of socket isnt initialized");
    char buffer[MAX_CLIENT_BUFFER_SIZE];
    int result = recv(socket, buffer, sizeof(buffer), 0);
    if (result == SOCKET_ERROR)
        throw std::runtime_error("read failed");

    return std::string(buffer, result);
}

void ServerSocket::write(const std::string & buffer) const {
    if (socket == SOCKET_ERROR)
        throw std::runtime_error("cant write buffer because of socket isnt initialized");

    int result = send(socket, buffer.c_str(), buffer.size() + 1, 0);

    if (result == SOCKET_ERROR)
        throw std::runtime_error("send failed");
}

void ServerSocket::close() {
    closesocket(socket);
}