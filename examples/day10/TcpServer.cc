#include "TcpServer.h"

#include <functional>
#include <cerrno>

#include "EventLoop.h"
#include "socket.h"
#include "Channel.h"
#include "logger.h"

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : loop_(loop), acceptor_(loop_, &addr) {
    // 将socket建立出来，并绑定事件后通过loop注册到epoll
    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
}

void TcpServer::newConnection(Socket* sock) {
    Connection* conn = new Connection(loop_, sock);
    conn->setDeleteConnectionCallBack(std::bind(&TcpServer::deleteConnection, this, std::placeholders::_1));
    connMap_[sock->fd()] = conn;
}

void TcpServer::deleteConnection(Socket* sock) {
    int fd = sock->fd();
    // 将连接移除
    if (connMap_.count(fd) == 0) { // 不存在
        return;
    }

    delete connMap_[fd];
    connMap_.erase(fd);
}