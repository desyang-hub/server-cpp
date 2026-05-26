#pragma once

#include "socket.h"

class EventLoop;
class Channel;

class TcpServer
{
private:
    EventLoop* loop_;
    Socket sock_;
public:
    TcpServer(EventLoop* loop, const InetAddress& addr);
    ~TcpServer() = default;

public:
    void connectCallBack();

    void messageCallBack(Channel* ch);
};
