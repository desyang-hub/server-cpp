#pragma once

#include "Epoll.h"
#include "Acceptor.h"
#include "nonecopyable.h"
#include "Channel.h"
#include "ThreadPool.h"

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
