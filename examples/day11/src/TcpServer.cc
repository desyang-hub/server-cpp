#include "TcpServer.h"

#include <functional>
#include <cerrno>

#include "EventLoop.h"
#include "socket.h"
#include "Channel.h"
#include "logger.h"

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : mainReactor_(loop), acceptor_(loop, &addr), pool_(std::thread::hardware_concurrency()), subReactors_(std::thread::hardware_concurrency()) {
    // 将socket建立出来，并绑定事件后通过loop注册到epoll
    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));

    // 将所有的subReactor 事件循环loop() 用 ThreadPool 运行起来
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        pool_.enqueue(&EventLoop::loop, &subReactors_[i]);
    }
}

void TcpServer::newConnection(int fd) {
    if (fd != -1) {
        int id = fd % subReactors_.size();

        ConnectionPtr connPtr = std::make_shared<Connection>(&subReactors_[id], fd);
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