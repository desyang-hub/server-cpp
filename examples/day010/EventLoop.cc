#include "EventLoop.h"

#include <iostream>

#include "Channel.h"

EventLoop::EventLoop() : epoll_(), isStop_(false) {
}

int EventLoop::epfd() const {
    return epoll_.epfd();
}

void EventLoop::loop() {
    while (!isStop_) {
        auto activeChannels = epoll_.poll();

        for (auto& ch : activeChannels) {
            ch->handleEvent();
        }
    }
}

void EventLoop::updateChannel(ChannelPtr chanPtr) {
    epoll_.updateChannel(chanPtr);
}

void EventLoop::removeChannel(int fd) {
    epoll_.removeChannel(fd);
}