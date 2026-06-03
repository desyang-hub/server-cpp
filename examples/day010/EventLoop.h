#pragma once

#include "Epoll.h"
#include "Acceptor.h"
#include "nonecopyable.h"
#include "Channel.h"
#include "ThreadPool.h"

#include <unordered_map>

class EventLoop : public nonecopyable
{
private:
    Epoll epoll_;
    bool isStop_;
    
    ThreadPool pool_;

    std::unordered_map<int, ChannelPtr> channels_;
public:
    EventLoop();
    ~EventLoop() = default;

    int epfd() const;

    void loop();

    void updateChannel(ChannelPtr);

    void removeChannel(int fd);

    template<class F>
    void submit(F&& f);
};

template<class F>
inline void EventLoop::submit(F&& f) {
    pool_.enqueue(std::forward<F>(f));
}
