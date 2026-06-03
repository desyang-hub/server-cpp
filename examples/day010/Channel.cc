#include "Channel.h"

#include "EventLoop.h"

#include <iostream>

Channel::Channel(EventLoop* loop, int fd, bool runInThreadPool_) : loop_(loop), fd_(fd), isInEpoll_(false), events_(0), revents_(0), eventCallBack_(nullptr), runInThreadPool_(runInThreadPool_) {

}

Channel::~Channel() {
    this->remove(); // 从epoll中将自己移除
}

int Channel::fd() const {
    return fd_;
}

int Channel::events() const {
    return events_;
}

void Channel::setEvents(int events) {
    events_ = events;
}

int Channel::revents() const {
    return revents_;
}

void Channel::setRevents(int revents) {
    revents_ = revents;
}

void Channel::update() {
    loop_->updateChannel(shared_from_this());
}

void Channel::enableRead() {
    events_ |= EPOLLIN;
}

void Channel::enableET() {
    events_ |= EPOLLET;
}

bool Channel::isInEpoll() const {
    return isInEpoll_;
}

void Channel::setInEpoll() {
    isInEpoll_ = true;
}

void Channel::remove() {
    loop_->removeChannel(fd_);
}

void Channel::setEventCallBack(const EventCallBack& cb) {
    eventCallBack_ = cb; // 拷贝构造
}

void Channel::handleEvent() {
    if (eventCallBack_) {
        if (runInThreadPool_) {
            loop_->submit(eventCallBack_);
        } else {
            eventCallBack_();
        }
    }
}