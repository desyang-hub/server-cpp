#pragma once

#include "Channel.h"
#include "Socket.h"

#include <functional>


class EventLoop;

class Connection
{
    using DeleteConnectionCallBack = std::function<void(Socket*)>;
private:
    EventLoop* loop_;
    ChannelPtr ch_;
    Socket sock_;
    DeleteConnectionCallBack deleteConnectionCallBack_;

public:
    Connection(EventLoop* loop);
    ~Connection() = default;

    void setDeleteConnectionCallBack(const DeleteConnectionCallBack&);

    void echo();
};