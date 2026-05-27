#include "Channel.h"
#include "EventLoop.h"

void Channel::update() {
    return loop_->updateChannel(this);
}

void Channel::enableReading() {
    events_ = EPOLLIN | EPOLLET;
    update();
}

void Channel::handelEvent() {
    if (readEventCallBack_) {
        loop_->execute(readEventCallBack_);
    }
}

void Channel::setReadEventCallBack(ReadEventCallBack cb) {
    readEventCallBack_ = std::move(cb);
}

void Channel::close() {
    if (!isServer() && fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Channel::remove() {
    loop_->removeChannel(this);
}