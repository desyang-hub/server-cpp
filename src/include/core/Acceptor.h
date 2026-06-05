/**
 * @FilePath     : /server-cpp/src/include/core/Acceptor.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:39:32
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include "net/Socket.h"
#include "core/Channel.h"

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