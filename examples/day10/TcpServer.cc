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

void TcpServer::newConnection(int fd) {
    if (fd != -1) {
        ConnectionPtr connPtr = std::make_shared<Connection>(loop_, fd);
        connPtr->initReadEventCallBack();
        connPtr->setDeleteConnectionCallBack(std::bind(&TcpServer::deleteConnection, this, std::placeholders::_1));

        
        std::lock_guard<std::mutex> lock(mutex_);
        errif(connMap_.count(fd), "conn exitst.");
        connMap_[fd] = connPtr;
    }
}

// FIXME： 只要有任务运行就不能够删除
void TcpServer::deleteConnection(int fd) {
    std::lock_guard<std::mutex> lock(mutex_);
    // 将连接移除
    if (connMap_.count(fd) == 0) { // 不存在
        return;
    }
    connMap_.erase(fd);
}