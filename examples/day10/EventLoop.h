#pragma once

#include "Epoll.h"
#include "ThreadPool.h"

#include <future>

class Channel;

class EventLoop
{
private:
    Epoll epoll_;
    ThreadPool pool_;
public:
     EventLoop() = default;
    ~EventLoop() = default;

public:
    void loop();

    void updateChannel(Channel*);

    void removeChannel(Channel*);

    template<class F, typename ...Args>
    std::future<typename std::result_of<F(Args...)>::type>
    execute(F&& f, Args&& ...args);
};

template<class F, typename ...Args>
inline std::future<typename std::result_of<F(Args...)>::type>
EventLoop::execute(F&& f, Args&& ...args) {
    return pool_.enqueue(std::forward<F>(f), std::forward<Args>(args)...);
}
