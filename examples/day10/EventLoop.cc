#include "EventLoop.h"

#include "Channel.h"
#include "logger.h"

void EventLoop::loop() {
    LOG_INFO << "server always luncher." << endl;
    while (true) {
        std::vector<Channel*> activeChannels = epoll_.poll();

        for (auto& chPtr : activeChannels) {
            chPtr->handelEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch) {
    epoll_.updateChannel(ch);
}

void EventLoop::removeChannel(ChannelPtr ch) {
    epoll_.removeChannel(ch);
}