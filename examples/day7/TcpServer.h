#pragma once

#include "socket.h"
#include "Acceptor.h"

class EventLoop;
class Channel;

class TcpServer
{
private:
    EventLoop* loop_;
    Acceptor acceptor_;
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer() = default;

public:
    void connectCallBack();

    void messageCallBack(Channel* ch);

    void newConnection(int fd);
};
