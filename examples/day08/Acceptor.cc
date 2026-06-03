#include "Acceptor.h"

#include "utils.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr) : loop_(loop), newConnectionCallBack_(nullptr), sock_(), chPtr_(nullptr) {
    sock_.bind(addr);
    sock_.listen();

    chPtr_ = std::make_unique<Channel>(loop, sock_.fd());
    chPtr_->enableRead();
    chPtr_->update();

    chPtr_->setEventCallBack(std::bind(&Acceptor::newConnectionCallBack, this));
}

void Acceptor::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}

void Acceptor::newConnectionCallBack() {
    if (newConnectionCallBack_) {
        newConnectionCallBack_(&sock_);
    }
}