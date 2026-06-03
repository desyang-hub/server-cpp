#pragma once

#include <sys/socket.h>
#include "utils.h"
#include "InetAddress.h"

class Socket
{
private:
    int fd_;
public:
    Socket() : fd_(::socket(AF_INET, SOCK_STREAM, 0)) {
        errif(fd_ == -1, "socket create error");
    }
    explicit Socket(int fd) : fd_(fd) {}
    ~Socket() {
        this->close();
    }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // 移动构造需要处理fd
    Socket(Socket&& sock) __THROW : fd_(sock.release()) {}
    Socket& operator=(Socket&& sock) __THROW {
        fd_ = sock.release();
        return *this;
    }

public:
    int fd() const { return fd_; }

    void close() {
        if (fd_ != -1) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    void bind(const InetAddress& addr) {
        errif(::bind(fd_, (sockaddr*)&addr.addr_, addr.sock_len_) == -1, "bind error");
    }

    void listen(int n = SOMAXCONN) {
        errif(::listen(fd_, n) == -1, "listen error");
    }

    int accept(InetAddress& addr) {
        int fd = ::accept(fd_, (sockaddr*)&addr.addr_, &addr.sock_len_);
        errif(fd == -1, "accept error");
        return fd;
    }

    void connect(const InetAddress& addr) {
        errif(::connect(fd_, (sockaddr*)&addr.addr_, addr.sock_len_) == -1, "connect error");
    }

    int send(const char* buf, int n) {
        return ::send(fd_, buf, n, 0);
    }

    int recv(char* buf, int max_len) {
        return ::recv(fd_, buf, max_len, 0);
    }

    int release() {
        int fd(fd_);
        fd_ = -1;
        return fd;
    }

    bool isValid() const {
        return fd_ != -1;
    }

    void setnoneblocking() {
        ::setnoneblocking(fd_);
    }

    void setTimeout(int seconds) {
        struct timeval tv = {seconds, 0}; // 1秒超时
        setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
        setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }
};
