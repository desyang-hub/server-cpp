#include "Acceptor.h"

#include "utils.h"

#include <iostream>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr) : loop_(loop), newConnectionCallBack_(nullptr), sock_(), chPtr_(nullptr) {
    sock_.bind(addr);
    sock_.listen();

    chPtr_ = std::make_unique<Channel>(loop, sock_.fd());
    chPtr_->enableRead();
    chPtr_->update();

    chPtr_->setEventCallBack([this]{
        if (newConnectionCallBack_)
            newConnectionCallBack_(&sock_);
    });
}

void Acceptor::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}