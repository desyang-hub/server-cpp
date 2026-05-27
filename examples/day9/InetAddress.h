#pragma once

#include <arpa/inet.h>
#include <string>
#include <strings.h>
#include <ostream>

struct InetAddress
{
    sockaddr_in addr_;
    socklen_t addrlen_;
public:
    explicit InetAddress(int port, const std::string& host = "127.0.0.1") : addrlen_(sizeof(addr_))
    {
        bzero(&addr_, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = inet_addr(host.c_str());
        addr_.sin_port = htons(port);
    }

    InetAddress() : addr_(sockaddr_in{}), addrlen_(sizeof(addr_)) {}

    ~InetAddress() = default;

public:
    std::string ip() const {
        return inet_ntoa(addr_.sin_addr);
    }

    int port() const {
        return ntohs(addr_.sin_port);
    }

    std::string ipaddr() const {
        return ip() + ":" + std::to_string(port());
    }

    friend std::ostream& operator<<(std::ostream& os, const InetAddress& addr) {
        return os << addr.ipaddr();
    }
};