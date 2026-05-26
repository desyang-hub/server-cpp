#pragma once

#include <stdint.h>

class Epoll;

class Channel
{
private:
    Epoll* epoll_;
    int fd_;
    uint32_t events_;    // 注册事件
    uint32_t revents_;   // 当前收到的事件
    bool is_inEpoll_;
public:
    Channel(Epoll* ep, int fd) : epoll_(ep), fd_(fd), events_(0), 
        revents_(0), is_inEpoll_(false) {}
    ~Channel() = default;

public:
    int fd() const {return fd_;}
    void setEvents(uint32_t events) {events_ = events;}
    uint32_t events() const {return events_;}
    void setREvents(uint32_t revents) {revents_ = revents;}
    uint32_t revents() const {return revents_;}
    bool isInEpoll() const {return is_inEpoll_;}
    void setIsInEpoll(bool isInEpoll) {is_inEpoll_ = isInEpoll;}

    bool update();
};
