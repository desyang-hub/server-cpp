/**
 * @FilePath     : /server-cpp/src/include/net/InetAddress.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:39:17
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

    explicit InetAddress(int port, const char* ip = "127.0.0.1");

public:
    std::string toIp() const;

    int toPort() const;

    std::string toIpPort() const;
};
