#include "Channel.h"

#include "Epoll.h"

Channel::Channel(Epoll* epoll, int fd) : epoll_(epoll), fd_(fd), isInEpoll_(false), events_(0), revents_(0) {

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
    epoll_->updateChannel(this);
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
    epoll_->removeChannel(this);
}