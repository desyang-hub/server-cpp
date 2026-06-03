#pragma once

#include "InetAddress.h"

class Socket
{
private:
    int fd_;
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // 移动构造需要处理fd
    Socket(Socket&& sock) __THROW;
    Socket& operator=(Socket&& sock) __THROW;

public:
    int fd() const;

    void close();

    void bind(const InetAddress& addr);

    void listen(int n = SOMAXCONN);

    int accept(InetAddress& addr);

    void connect(const InetAddress& addr);

    int send(const char* buf, int n);

    int recv(char* buf, int max_len);

    int release();

    bool isValid() const;

    void setnoneblocking();

    void setTimeout(int seconds);

    void setInetAddrReuse();
};
