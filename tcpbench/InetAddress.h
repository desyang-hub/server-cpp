/**
 * @FilePath     : /server-cpp/test/InetAddress.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 14:42:00
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 14:42:01
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
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

    explicit InetAddress(int port, const char* ip = "127.0.0.1") : sock_len_(sizeof(addr_)) {
        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = inet_addr(ip);
        addr_.sin_port = htons(port);
    }

public:
    std::string toIp() const {
        return inet_ntoa(addr_.sin_addr);
    }

    int toPort() const {
        return ntohs(addr_.sin_port);
    }

    std::string toIpPort() const {
        return toIp() + ":" + std::to_string(toPort());
    }
};
