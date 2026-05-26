#include "socket.h"

#include <unistd.h>

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd_ != -1);
}

Socket::~Socket()
{
    this->close();
}


bool Socket::bind(int port, const std::string& host) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    server_addr.sin_port = htons(port);

    return ::bind(fd_, (sockaddr*)&server_addr, sizeof(server_addr)) == -1;
}

bool Socket::listen(int n) {
    return ::listen(fd_, n) != -1;
}

int Socket::accept() {
    sockaddr_in server_addr{};
    socklen_t len = sizeof(server_addr);
    int client_fd = ::accept(fd_, (sockaddr*)&server_addr, &len);
    assert(client_fd != -1);

    return client_fd;
}

bool Socket::connect(int port, const std::string& host) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    server_addr.sin_port = htons(port);

    return ::connect(fd_, (sockaddr*)&server_addr, sizeof(server_addr)) != -1;
}


bool Socket::send(const std::string& msg) {
    return (::send(fd_, msg.c_str(), msg.size(), 0)) != -1;
}

int Socket::recv(char*& buf, size_t len) {
    return ::recv(fd_, buf, len, 0);
}

void Socket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}