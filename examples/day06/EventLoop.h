#pragma once

#include "Epoll.h"
#include "nonecopyable.h"

class Channel;

class EventLoop : public nonecopyable
{
private:
    Epoll epoll_;
    bool isStop_;
public:
    EventLoop();
    ~EventLoop() = default;

    int epfd() const;

    void loop();

    void updateChannel(Channel*);

    void removeChannel(Channel*);
};
