#pragma once

#include "Channel.h"
#include "Socket.h"
#include "Buffer.h"
#include "nonecopyable.h"

#include <functional>
#include <memory>
#include <iostream>

class EventLoop;

class Connection : public std::enable_shared_from_this<Connection>
{
    using DeleteConnectionCallBack = std::function<void(int)>;
    using OnMessageCallBack = std::function<void(Connection*, Buffer*)>;
private:
    EventLoop* loop_;
    Socket sock_;
    Channel ch_;
    Buffer readBuffer_;
    DeleteConnectionCallBack deleteConnectionCallBack_;
    OnMessageCallBack onMessageCallBack_;

    void handelEvent();

public:
    Connection(EventLoop* loop, int fd, bool runInThreadPool = false);
    ~Connection() = default;

    int fd() const;

    void initReadEventCallBack();

    void setDeleteConnectionCallBack(const DeleteConnectionCallBack&);

    void setOnMessageCallBack(const OnMessageCallBack&);

    void close();

    int send(const std::string&);

    int send(const char* buf, size_t len);
};

using ConnectionPtr = std::shared_ptr<Connection>;