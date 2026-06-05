/**
 * @FilePath     : /server-cpp/src/net/InetAddress.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:38:46
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "net/InetAddress.h"


InetAddress::InetAddress(int port, const char* ip) : sock_len_(sizeof(addr_)) {
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}

std::string InetAddress::toIp() const {
    return inet_ntoa(addr_.sin_addr);
}

int InetAddress::toPort() const {
    return ntohs(addr_.sin_port);
}

std::string InetAddress::toIpPort() const {
    return toIp() + ":" + std::to_string(toPort());
}