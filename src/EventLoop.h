#pragma once

class Epoll;

class EventLoop
{
private:
    Epoll* epoll_;
public:
    EventLoop(Epoll* epoll) : epoll_(epoll) {}
    ~EventLoop();

public:
    void loop();
};