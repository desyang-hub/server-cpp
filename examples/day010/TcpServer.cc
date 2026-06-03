#include "TcpServer.h"

#include <iostream>

#include "utils.h"


TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : loop_(loop), acceptor_(loop, addr) {
    // 多线程可能异常
    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnectionCallBack, this, std::placeholders::_1));
}

void TcpServer::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}

void TcpServer::newConnectionCallBack(Socket* sock) {
    InetAddress addr;
    int fd = sock->accept(addr);

    std::cout << "connection user=" << fd << std::endl;

    // 单线程暂时无需考虑数据竞争问题
    connections_[fd] = std::make_shared<Connection>(loop_, fd);

    connections_[fd]->setDeleteConnectionCallBack(std::bind(&TcpServer::disConnectionCallBack, this, std::placeholders::_1));
}

void TcpServer::disConnectionCallBack(Socket* sock) {
    std::cout << "user fd=" << sock->fd() << " disconnected." << std::endl;
    // 数据保护
    if (connections_.count(sock->fd())) {
        connections_.erase(sock->fd());
    }
}