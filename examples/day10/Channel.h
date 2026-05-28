#pragma once

#include <stdint.h> 
#include <functional>
#include <memory>
#include "utils.h"

class EventLoop;

class Channel : public std::enable_shared_from_this<Channel>
{
    using ReadEventCallBack = std::function<void()>;
private:
    EventLoop* loop_;
    int fd_;
    
    uint32_t events_;    // 注册事件
    uint32_t revents_;   // 当前收到的事件
    bool is_inEpoll_;
    bool usePool_;

    ReadEventCallBack readEventCallBack_;

public:
    Channel(EventLoop* loop, int fd, bool usePool = true) : loop_(loop), fd_(fd), 
        events_(0), revents_(0), is_inEpoll_(false), readEventCallBack_(nullptr), usePool_(usePool) {
            errif(fd_ == -1, "Invaild fd");
        }
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


using ChannelPtr = std::shared_ptr<Channel>;