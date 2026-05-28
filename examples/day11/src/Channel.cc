#include "Channel.h"
#include "EventLoop.h"
#include "logger.h"

void Channel::update() {
    return loop_->updateChannel(this);
}

void Channel::enableReading() {
    events_ = EPOLLIN | EPOLLET;
    update();
}

void Channel::handelEvent() {
    
    if (readEventCallBack_) {
        if (usePool_) {
            loop_->execute(readEventCallBack_);
        }
        else
            readEventCallBack_();
    }
}

void Channel::setReadEventCallBack(ReadEventCallBack cb) {
    readEventCallBack_ = std::move(cb);
}

void Channel::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Channel::remove() {
    loop_->removeChannel(shared_from_this());
}