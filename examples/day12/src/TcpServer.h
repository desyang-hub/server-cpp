#pragma once

#include "InetAddress.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Socket.h"

#include <functional>
#include <unordered_map>
#include <mutex>

class EventLoop;
class Socket;

class TcpServer
{
    using NewConnectionCallBack = std::function<void(int)>;
private:
    EventLoop* loop_;
    Acceptor acceptor_;
    std::unordered_map<int, ConnectionPtr> connections_;

    NewConnectionCallBack new_connection_call_back_;

    mutable std::mutex mutex_;
public:
    TcpServer(EventLoop* loop, const InetAddress& listen_addr);
    ~TcpServer() = default;

public:
    void setNewConnectionCallBack(const NewConnectionCallBack& cb) {
        new_connection_call_back_ = cb;
    }

    void connectionCallBack(int fd) {
        std::lock_guard<std::mutex> lock(mutex_);

        errif(connections_.count(fd) != 0, "connection always exists");
        auto conn = std::make_shared<Connection>(loop_, fd);
        conn->initEventCallBack();

        connections_[fd] = conn;
    }

    void disconnectCallBack(int fd) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = connections_.find(fd);
        if (it != connections_.end()) {
            connections_.erase(it);
        }
    }
};