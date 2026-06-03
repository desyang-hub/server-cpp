#pragma once

#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
#include "nonecopyable.h"

#include <functional>

class EventLoop;

class Connection : public nonecopyable
{
    using DeleteConnectionCallBack = std::function<void(Socket*)>;
private:
    EventLoop* loop_;
    ChannelPtr ch_;
    Socket sock_;
    Buffer readBuffer_;
    DeleteConnectionCallBack deleteConnectionCallBack_;

public:
    Connection(EventLoop* loop, int);
    ~Connection() = default;

    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection&&) noexcept = default;

    void setDeleteConnectionCallBack(const DeleteConnectionCallBack&);

    void echo();
};

using ConnectionPtr = std::shared_ptr<Connection>;