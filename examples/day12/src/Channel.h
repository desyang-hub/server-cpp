#pragma once

#include <functional>
#include <sys/epoll.h>
#include <memory>

class EventLoop;

class Channel
{
    using EventCallback = std::function<void()>;
private:
    EventLoop* loop_;
    int fd_;
    int events_; // 监听的事件
    int revents_;
    bool isInEpoll_;

    EventCallback eventCallback_;
public:
    Channel(EventLoop* epoll, int fd) : loop_(epoll), fd_(fd), events_(0), revents_(0), isInEpoll_(false) {}
    ~Channel() = default;

public:
    int fd() const { return fd_; }

    int events() const { return events_; }
    void set_events(int events) { events_ = events; }

    int revents() const { return revents_; }
    void set_revents(int revents) { revents_ = revents; }

    bool is_in_epoll() const { return isInEpoll_; }
    void set_in_epoll() { isInEpoll_ = true; }

    void enable_reading() {
        events_ |= (EPOLLIN | EPOLLET);
        update();
    }

    void update();

    void set_event_callback(const EventCallback& cb) { eventCallback_ = cb; }
    void handle_event() {
        if (eventCallback_) {
            eventCallback_();
        }
    }
};

using ChannelPtr = std::unique_ptr<Channel>;