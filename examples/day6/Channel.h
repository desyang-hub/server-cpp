#pragma once

#include <stdint.h> 
#include <functional>

class EventLoop;


class Channel
{
    using ReadEventCallBack = std::function<void(Channel*)>;
private:
    EventLoop* loop_;
    int fd_;

    bool is_server_;
    uint32_t events_;    // 注册事件
    uint32_t revents_;   // 当前收到的事件
    bool is_inEpoll_;

    ReadEventCallBack readEventCallBack_;

    bool isServer() const {
        return is_server_;
    }

public:
    Channel(EventLoop* loop, int fd, bool is_server = false) : loop_(loop), fd_(fd), 
        events_(0), revents_(0), is_inEpoll_(false),
        is_server_(is_server), readEventCallBack_(nullptr) {}
    ~Channel() {
        this->close();
    }

public:
    int fd() const {return fd_;}
    void setEvents(uint32_t events) {events_ = events;}
    uint32_t events() const {return events_;}
    void setREvents(uint32_t revents) {revents_ = revents;}
    uint32_t revents() const {return revents_;}
    bool isInEpoll() const {return is_inEpoll_;}
    void setIsInEpoll(bool isInEpoll) {is_inEpoll_ = isInEpoll;}

    void update();

    void enableReading();

    void handelEvent();
    void setReadEventCallBack(ReadEventCallBack cb);

    void close();

    void remove();
};
