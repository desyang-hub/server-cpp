#pragma once

#include <unordered_map>

#include "socket.h"
#include "Acceptor.h"
#include "Connection.h"

class EventLoop;
class Channel;

class TcpServer
{
private:
    EventLoop* loop_;
    Acceptor acceptor_;
    std::unordered_map<int, Connection*> connMap_;
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer() {
        // 将connMap_ 中的所有元素回收
        for (auto& p : connMap_) {
            delete p.second;
            p.second = nullptr;
        }
    }

public:
    void newConnection(Socket* sock);

    void deleteConnection(Socket* sock);
};
