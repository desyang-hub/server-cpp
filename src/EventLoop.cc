#include "EventLoop.h"

#include "Epoll.h"
#include "Channel.h"

void EventLoop::loop() {
    while (true) {
        auto activeChannels = epoll_->poll();
        for (auto& ch : activeChannels) {
            if (ch->revents() & EPOLLIN) {
                // 处理读事件
                ch->handle_event();
            }
        }
    }
}