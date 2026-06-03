#pragma once

#include <arpa/inet.h>
#include <string>

struct InetAddress
{
    sockaddr_in addr_;
    socklen_t sock_len_;
public:
    InetAddress() : addr_{}, sock_len_(sizeof(addr_)) {}
    ~InetAddress() = default;

    explicit InetAddress(int port, const char* ip = "127.0.0.1");

public:
    std::string toIp() const;

    int toPort() const;

    std::string toIpPort() const;
};
