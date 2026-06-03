#pragma once

#include "Epoll.h"
#include "Acceptor.h"
#include "nonecopyable.h"
#include "Channel.h"

#include <unordered_map>

class EventLoop : public nonecopyable
{
private:
    Epoll epoll_;
    bool isStop_;

    std::unordered_map<int, ChannelPtr> channels_;
public:
    EventLoop();
    ~EventLoop() = default;

    int epfd() const;

    void loop();

    void updateChannel(Channel*);

    void removeChannel(Channel*);
};
