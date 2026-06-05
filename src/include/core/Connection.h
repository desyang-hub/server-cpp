/**
 * @FilePath     : /server-cpp/src/include/core/Connection.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:39:40
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include "core/Channel.h"
#include "net/Socket.h"
#include "base/Buffer.h"
#include "common/nonecopyable.h"

#include <functional>
#include <memory>

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