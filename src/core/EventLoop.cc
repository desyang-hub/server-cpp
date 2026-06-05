/**
 * @FilePath     : /server-cpp/src/core/EventLoop.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:34
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "core/EventLoop.h"

#include "core/Channel.h"

/// @brief 
/// @param threadPoolSize: 默认值`0`表示不启用线程池
EventLoop::EventLoop(size_t threadPoolSize) : epoll_(), isStop_(false), threadPoolSize_(threadPoolSize) {
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

void EventLoop::removeChannel(int fd) {
    epoll_.removeChannel(fd);
}