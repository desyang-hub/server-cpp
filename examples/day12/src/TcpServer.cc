#include "TcpServer.h"

#include "EventLoop.h"

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listen_addr) : acceptor_(loop, listen_addr), loop_(loop) {
    // 创建acceptor用于监听新连接
    // acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::connectionCallBack, this, std::placeholders::_1));

    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::connectionCallBack, this, std::placeholders::_1));
}