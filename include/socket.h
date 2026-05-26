#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string>

#include "utils.h"

class Socket
{
private:
    int fd_;
public:
    Socket();
    ~Socket();

public: // 列出所有的步骤
    bool bind(int port, const std::string& host = "127.0.0.1");

    bool listen(int n);

    // return: client_fd
    int accept();

    bool connect(int port, const std::string& host = "127.0.0.1");

    bool send(const std::string&);

    int recv(char*& buf, size_t len);

    void close();
};

