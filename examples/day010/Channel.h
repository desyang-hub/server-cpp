#pragma once

#include <functional>
#include <memory>

class EventLoop;

class Channel
{
    using EventCallBack = std::function<void()>;
private:
    EventLoop* loop_;
    int fd_;
    int events_;
    int revents_;
    bool isInEpoll_;
    EventCallBack eventCallBack_;
    bool runInThreadPool_;

public:
    Channel(EventLoop* loop, int fd, bool runInThreadPool = false);
    ~Channel();

    int fd() const;

    int events() const;

    void setEvents(int events);

    int revents() const;

    void setRevents(int revents);

    void update();

    void enableRead();

    void enableET();

    bool isInEpoll() const;

    void setInEpoll();

    void remove();

    void setEventCallBack(const EventCallBack& cb);

    void handleEvent();
};

using ChannelPtr = std::unique_ptr<Channel>;
