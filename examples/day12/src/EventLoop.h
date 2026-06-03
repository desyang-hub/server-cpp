#pragma once

#include "Epoll.h"
#include "Channel.h"

// 用于管理epoll事件循环，分发事件到对应的处理函数
class EventLoop
{
private:
    Epoll epoll_;
public:
    EventLoop() : epoll_() {}
    ~EventLoop() = default;

public:
    void loop() {
        while (true) {
            auto activeChannels = epoll_.poll();

            for (auto& ch : activeChannels) {
                // 可以根据具体的感兴趣事件来触发不同的调用
                if (ch->revents() & EPOLLIN) {
                    ch->handle_event();
                }
            }
        }
    }

    void removeChannel(Channel* channel) {
        epoll_.removeChannel(channel);
    }

    void updateChannel(Channel* channel) {
        epoll_.updateChannel(channel);
    }
};