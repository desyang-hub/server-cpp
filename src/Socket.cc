#include "Socket.h"

#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

#include "utils.h"
#include "Buffer.h"

Socket::Socket() : fd_(::socket(AF_INET, SOCK_STREAM, 0)), isNoneBlocking_(false) {
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

/// @brief 将消息通过fd进行发送（适用于阻塞socket）
/// @param buf 数据源（必须有效）
/// @param n 需要发送的数据长度（必须 > 0）
/// @return 返回 -1 表示发生不可恢复错误（如连接断开），否则返回实际发送的字节数（可能 < n）
int Socket::send(const char* buf, int n) {
    if (n <= 0) return 0;

    int total_sent = 0;
    while (total_sent < n) {
        ssize_t num_send = ::send(fd_, buf + total_sent, n - total_sent, 0);

        if (num_send > 0) {
            total_sent += num_send;
            continue;
        }

        if (num_send == -1) {
            if (errno == EINTR) {
                continue; // 信号中断，重试
            }

            // 关键：非阻塞模式下 EAGAIN 是正常的，应返回已发送量
            if (isNoneBlocking() && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                break; // 暂时无法发送，返回当前进度
            }

            // 其他错误（包括阻塞模式下的 EAGAIN，视为异常）
            return -1;
        }

        // send 返回 0（TCP 中几乎不可能，视为异常）
        return -1;
    }

    return total_sent;
}

int Socket::send(const std::string& buf) {
    return this->send(buf.c_str(), buf.size());
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
    isNoneBlocking_ = true;
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

bool Socket::isNoneBlocking() const {
    return isNoneBlocking_;
}