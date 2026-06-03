#pragma once

#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
#include "nonecopyable.h"

#include <functional>
#include <memory>

class EventLoop;

class Connection : public std::enable_shared_from_this<Connection>
{
    using DeleteConnectionCallBack = std::function<void(int)>;
private:
    EventLoop* loop_;
    ChannelPtr ch_;
    Socket sock_;
    Buffer readBuffer_;
    DeleteConnectionCallBack deleteConnectionCallBack_;

public:
    Connection(EventLoop* loop, int fd, bool runInThreadPool = false);
    ~Connection() = default;

    void initReadEventCallBack();

    void setDeleteConnectionCallBack(const DeleteConnectionCallBack&);

    void echo();
};

using ConnectionPtr = std::shared_ptr<Connection>;