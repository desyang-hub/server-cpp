/**
 * @FilePath     : /server-cpp/src/include/core/EventLoop.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:39:49
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include "core/Epoll.h"
#include "core/Acceptor.h"
#include "common/nonecopyable.h"
#include "core/Channel.h"
#include "base/ThreadPool.h"

#include <unordered_map>
#include <stdint.h>

class EventLoop : public nonecopyable
{
private:
    Epoll epoll_;
    bool isStop_;
    size_t threadPoolSize_;

public:
    explicit EventLoop(size_t threadPoolSize_ = 0);
    ~EventLoop() = default;

    int epfd() const;

    void loop();

    void updateChannel(Channel*);

    void removeChannel(int fd);

    /// @brief 提交到EventLoop执行任务
    /// @tparam F 
    /// @param f
    template<class F>
    void submit(F&& f);
};

template<class F>
inline void EventLoop::submit(F&& f) {
    if (threadPoolSize_) {
        static ThreadPool pool_(threadPoolSize_);
        pool_.enqueue(std::forward<F>(f));
    } else {
        f();
    }
}
