#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Channel.h"
#include "logger.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress* addr) : loop_(loop), addr_(addr), sock_(), newConnectionCallBack_(nullptr), ch_(nullptr) {
    sock_.bind(*addr);
    sock_.listen();

    int fd = sock_.fd();

    ch_ = std::make_shared<Channel>(loop_, fd, false);
    ch_->setEvents(EPOLLIN);
    // ch->enableReading();
    ch_->setReadEventCallBack(std::bind(&Acceptor::acceptConnection, this));
    ch_->update();
}

void Acceptor::acceptConnection() {
    InetAddress addr = InetAddress();
    
    int fd = sock_.accept(addr);

    // LOG_INFO << "user fd " << fd << " " << " ip addr: " << addr << endl;

    if (newConnectionCallBack_)
        newConnectionCallBack_(fd);
}

void Acceptor::setNewConnectionCallBack(NewConnectionCallBack cb) {
    newConnectionCallBack_ = std::move(cb);
}