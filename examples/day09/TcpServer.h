#pragma once

#include "Acceptor.h"
#include "Connection.h"


#include <functional>
#include <unordered_map>

class EventLoop;

class TcpServer
{
    using NewConnectionCallBack = std::function<void(Socket*)>;
    using DisConnectionCallBack = std::function<void(Socket*)>;
private:
    Acceptor acceptor_;
    EventLoop* loop_;
    std::unordered_map<int, ConnectionPtr> connections_;

    NewConnectionCallBack newConnectionCallBack_;
    DisConnectionCallBack disConnectionCallBack_;
public:
    TcpServer( EventLoop* loop, const InetAddress& addr);
    ~TcpServer() = default;

    void setNewConnectionCallBack(const NewConnectionCallBack& cb);

    void newConnectionCallBack(Socket*);

    void setDisConnectionCallBack(const DisConnectionCallBack& cb);

    void disConnectionCallBack(Socket*);

    void echo(int);
};