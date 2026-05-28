#include "socket.h"

#include <unistd.h>

#include "logger.h"
#include "utils.h"

Socket::Socket()
{
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error");
}

Socket::Socket(int fd) : fd_(fd) {}

Socket::~Socket()
{
    this->close();
}

int Socket::bind(const InetAddress& addr) {
    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

    int num = ::bind(fd_, (sockaddr*)&addr.addr_, addr.addrlen_);
    errif(num == -1, "bind error");
    return num;
}

int Socket::listen(int n) {
    int num = ::listen(fd_, n);
    errif(num == -1, "listen error");
    return num;
}

int Socket::accept(InetAddress& addr) {
    int client_fd = ::accept(fd_, (sockaddr*)&addr.addr_, &addr.addrlen_);
    errif(client_fd == -1, "accept error");

    return client_fd;
}

bool Socket::connect(const InetAddress& addr) {
    return ::connect(fd_, (sockaddr*)&addr.addr_, addr.addrlen_) != -1;
}

// 这里用拷贝，避免异常发生
int Socket::send(std::string msg) {
    return ::send(fd_, msg.c_str(), msg.size(), 0);
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

void Socket::setTimeout(int seconds) {
    struct timeval tv = {seconds, 0}; // 1秒超时
    setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}