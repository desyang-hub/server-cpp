#include "socket.h"

#include <unistd.h>

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd_ != -1);
}

Socket::Socket(int fd) : fd_(fd) {}

Socket::~Socket()
{
    this->close();
}

bool Socket::bind(const InetAddress& addr) {
    return ::bind(fd_, (sockaddr*)&addr.addr_, addr.addrlen_) != -1;
}

bool Socket::listen(int n) {
    return ::listen(fd_, n) != -1;
}

Socket Socket::accept(InetAddress& addr) {
    int client_fd = ::accept(fd_, (sockaddr*)&addr.addr_, &addr.addrlen_);
    assert(client_fd != -1);

    return Socket(client_fd);
}

bool Socket::connect(const InetAddress& addr) {
    return ::connect(fd_, (sockaddr*)&addr.addr_, addr.addrlen_) != -1;
}


bool Socket::send(const std::string& msg) {
    return (::send(fd_, msg.c_str(), msg.size(), 0)) != -1;
}

int Socket::recv(char* buf, size_t len) {
    return ::recv(fd_, buf, len, 0);
}

void Socket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

// 释放自己所持有的fd
int Socket::release() {
    int fd = fd_;
    fd_ = -1;
    return fd;
}

void Socket::setnoneblocking() {
    ::setnoneblocking(fd_);
}