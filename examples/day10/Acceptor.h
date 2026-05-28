#pragma once

#include "socket.h"
#include <functional>
#include "Channel.h"

using NewConnectionCallBack = std::function<void(int)>;

class EventLoop;
class InetAddress;

class Acceptor
{
private:
    EventLoop* loop_;
    ChannelPtr ch_;
    const InetAddress* addr_;

    Socket sock_;
    NewConnectionCallBack newConnectionCallBack_;
public:
    Acceptor(EventLoop* loop, const InetAddress* addr);
    ~Acceptor() = default;

public:
    void acceptConnection();

    void setNewConnectionCallBack(NewConnectionCallBack cb);
};