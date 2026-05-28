#pragma once

#include "Epoll.h"
#include "ThreadPool.h"

#include <future>
#include <memory>

class Channel;

class EventLoop
{
private:
    Epoll epoll_;
    // ThreadPool pool_;
    
public:
    EventLoop() {
    }
    ~EventLoop() = default;

public:
    void loop();

    void updateChannel(Channel* ch);

    void removeChannel(ChannelPtr ch);

    // template<class F, typename ...Args>
    // std::future<typename std::result_of<F(Args...)>::type>
    // execute(F&& f, Args&& ...args);
};

// template<class F, typename ...Args>
// inline std::future<typename std::result_of<F(Args...)>::type>
// EventLoop::execute(F&& f, Args&& ...args) {
//     return pool_.enqueue(std::forward<F>(f), std::forward<Args>(args)...);
// }

using EventLoopPtr = std::shared_ptr<EventLoop>;