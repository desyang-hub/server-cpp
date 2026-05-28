#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string>

#include "utils.h"
#include "InetAddress.h"

class Socket
{
private:
    int fd_;
public:
    Socket();
    Socket(int fd);
    ~Socket();

    int fd() const {
        return fd_;
    }

public: // 列出所有的步骤
    int bind(const InetAddress&);

    int listen(int n = SOMAXCONN);

    // return: client_fd
    int accept(InetAddress& addr);

    bool connect(const InetAddress&);

    int send(std::string);

    int recv(char* buf, size_t len);

    void close();

    int release();

    void setnoneblocking();

    void setTimeout(int seconds);
};

