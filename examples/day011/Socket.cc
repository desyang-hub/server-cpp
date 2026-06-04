#include "Socket.h"

#include <unistd.h>
#include <sys/socket.h>

#include "utils.h"

Socket::Socket() : fd_(::socket(AF_INET, SOCK_STREAM, 0)) {
    errif(fd_ == -1, "socket create error");
}

Socket::Socket(int fd) : fd_(fd) {}

Socket::~Socket() {
    this->close();
}

// 移动构造需要处理fd
Socket::Socket(Socket&& sock) __THROW : fd_(sock.release()) {}


Socket& Socket::operator=(Socket&& sock) __THROW {
    fd_ = sock.release();
    return *this;
}

int Socket::fd() const { return fd_; }

void Socket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Socket::bind(const InetAddress& addr) {
    errif(::bind(fd_, (sockaddr*)&addr.addr_, addr.sock_len_) == -1, "bind error");
}

void Socket::listen(int n) {
    errif(::listen(fd_, n) == -1, "listen error");
}

int Socket::accept(InetAddress& addr) {
    int fd = ::accept(fd_, (sockaddr*)&addr.addr_, &addr.sock_len_);
    errif(fd == -1, "accept error");
    return fd;
}

void Socket::connect(const InetAddress& addr) {
    errif(::connect(fd_, (sockaddr*)&addr.addr_, addr.sock_len_) == -1, "connect error");
}

int Socket::send(const char* buf, int n) {
    return ::send(fd_, buf, n, 0);
}

int Socket::send(const std::string& buf) {
    return ::send(fd_, buf.data(), buf.size(), 0);
}

int Socket::recv(char* buf, int max_len) {
    return ::recv(fd_, buf, max_len, 0);
}

int Socket::release() {
    int fd(fd_);
    fd_ = -1;
    return fd;
}

bool Socket::isValid() const {
    return fd_ != -1;
}

void Socket::setnoneblocking() {
    ::setnoneblocking(fd_);
}

void Socket::setTimeout(int seconds) {
    struct timeval tv = {seconds, 0}; // 1秒超时
    setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

void Socket::setInetAddrReuse() {
    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}