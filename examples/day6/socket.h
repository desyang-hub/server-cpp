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
    bool bind(const InetAddress&);

    bool listen(int n);

    // return: client_fd
    Socket accept(InetAddress& addr);

    bool connect(const InetAddress&);

    bool send(const std::string&);

    int recv(char* buf, size_t len);

    void close();

    int release();

    void setnoneblocking();
};

