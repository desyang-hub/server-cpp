#pragma once

#include "Socket.h"
#include "Channel.h"

#include <functional>

class EventLoop;

class Acceptor
{
    using NewConnectionCallBack = std::function<void(Socket*)>;
private:
    Socket sock_;
    EventLoop* loop_;
    ChannelPtr chPtr_;
    NewConnectionCallBack newConnectionCallBack_;
public:
    Acceptor(EventLoop* loop, const InetAddress& addr);
    ~Acceptor() = default;

    void setNewConnectionCallBack(const NewConnectionCallBack& cb);
};