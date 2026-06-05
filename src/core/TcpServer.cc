/**
 * @FilePath     : /server-cpp/src/core/TcpServer.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:38
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "core/TcpServer.h"

#include "common/utils.h"
#include "core/EventLoop.h"


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

    ConnectionPtr connPtr = std::make_shared<Connection>(&subReactors_[id], fd);
    connPtr->initReadEventCallBack();
    connPtr->setDeleteConnectionCallBack(std::bind(&TcpServer::disConnectionCallBack, this, std::placeholders::_1));
    connPtr->setOnMessageCallBack(onMessageCallBack_);

    {
        std::lock_guard<std::mutex> lock(mutex_);
        errif(connections_.count(fd) != 0, "connection always exists.");
        connections_[fd] = connPtr;
    }

    // 考虑一下这里是否会有并发问题
    if (onConnectionCallBack_) {
        onConnectionCallBack_(connPtr.get());
    }
}

void TcpServer::disConnectionCallBack(int fd) {
    std::cout << "user fd=" << fd << " disconnected." << std::endl;
    std::lock_guard<std::mutex> lock(mutex_);
    // 数据保护
    if (connections_.count(fd)) {
        connections_.erase(fd);
    }
}

void TcpServer::setOnConnection(const OnConnectionCallBack& onConnection) {
    onConnectionCallBack_ = onConnection;
}

void TcpServer::setOnMessageCallBack(const OnMessageCallBack& cb) {
    onMessageCallBack_ = cb;
}