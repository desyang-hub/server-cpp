#include "EventLoop.h"

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

void EventLoop::updateChannel(Channel* ch) {
    epoll_.updateChannel(ch);
}

void EventLoop::removeChannel(Channel* ch) {
    epoll_.removeChannel(ch);
}