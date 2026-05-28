#pragma once

#include <unordered_map>
#include <mutex>
#include <memory>

#include "socket.h"
#include "Acceptor.h"
#include "Connection.h"

class EventLoop;
class Channel;

using ConnectionPtr = std::shared_ptr<Connection>;

class TcpServer
{
private:
    EventLoop* loop_;
    Acceptor acceptor_;
    std::unordered_map<int, ConnectionPtr> connMap_;

    mutable std::mutex mutex_;
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer() = default;

public:
    void newConnection(int fd);

    void deleteConnection(int fd);
};
