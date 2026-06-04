#include "TcpServer.h"

#include <iostream>

#include "utils.h"
#include "EventLoop.h"


TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : acceptor_(loop, addr), mainReactor_(loop), threadPool_(HARDWARE_CONCURRENCY), subReactors_(HARDWARE_CONCURRENCY) {
    // 多线程可能异常
    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnectionCallBack, this, std::placeholders::_1));

    for (int i = 0; i < HARDWARE_CONCURRENCY; ++i) {
        // 将subReactor用线程池启动
        threadPool_.enqueue(&EventLoop::loop, &subReactors_[i]);
    }
}

void TcpServer::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}

void TcpServer::newConnectionCallBack(Socket* sock) {
    InetAddress addr;
    // mainReactor只需要处理accept，所以是单线程，无需考虑sock占用问题
    int fd = sock->accept(addr);

    // 更具fd随机分配到subReactor
    int id = fd % subReactors_.size();

    std::lock_guard<std::mutex> lock(mutex_);
    errif(connections_.count(fd) != 0, "connection always exists.");

    connections_[fd] = std::make_shared<Connection>(&subReactors_[id], fd);
    connections_[fd]->initReadEventCallBack();

    connections_[fd]->setDeleteConnectionCallBack(std::bind(&TcpServer::disConnectionCallBack, this, std::placeholders::_1));
}

void TcpServer::disConnectionCallBack(int fd) {
    std::cout << "user fd=" << fd << " disconnected." << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    // 数据保护
    if (connections_.count(fd)) {
        connections_.erase(fd);
    }
}