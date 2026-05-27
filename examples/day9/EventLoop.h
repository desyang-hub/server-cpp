#pragma once

#include "Epoll.h"

class Channel;

class EventLoop
{
private:
    Epoll epoll_;
public:
     EventLoop() = default;
    ~EventLoop() = default;

public:
    void loop();

    void updateChannel(Channel*);

    void removeChannel(Channel*);
};
