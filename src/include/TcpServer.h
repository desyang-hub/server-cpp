#pragma once

#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"

#include <functional>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <thread>

class EventLoop;
class Buffer;

namespace {
    const int HARDWARE_CONCURRENCY = std::thread::hardware_concurrency();
}

class TcpServer
{
    using NewConnectionCallBack = std::function<void(Socket*)>;
    using DisConnectionCallBack = std::function<void(Socket*)>;
    using OnConnectionCallBack = std::function<void(Connection*)>;
    using OnMessageCallBack = std::function<void(Connection*, Buffer*)>;
private:
    Acceptor acceptor_;
    EventLoop* mainReactor_;
    ThreadPool threadPool_;
    std::vector<EventLoop> subReactors_;
    std::unordered_map<int, ConnectionPtr> connections_;
    

    NewConnectionCallBack   newConnectionCallBack_;
    DisConnectionCallBack   disConnectionCallBack_;
    OnConnectionCallBack    onConnectionCallBack_;
    OnMessageCallBack       onMessageCallBack_;

    mutable std::mutex mutex_;
public:
    TcpServer( EventLoop* loop, const InetAddress& addr);
    ~TcpServer() = default;

    void setNewConnectionCallBack(const NewConnectionCallBack& cb);

    void newConnectionCallBack(Socket*);

    void setDisConnectionCallBack(const DisConnectionCallBack& cb);

    void disConnectionCallBack(int fd);

    void setOnConnection(const OnConnectionCallBack& onConnection);

    void setOnMessageCallBack(const OnMessageCallBack&);
};