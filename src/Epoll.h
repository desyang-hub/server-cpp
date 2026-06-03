#pragma once

#include <sys/epoll.h>
#include <vector>

#include "utils.h"

class Channel;

const int EPOLL_EVENT_DEFAULT_SIZE = 1024;

class Epoll
{
private:
    int epfd_;
    std::vector<epoll_event> events_;
    
public:
    Epoll(int epoll_event_size = EPOLL_EVENT_DEFAULT_SIZE) : epfd_(::epoll_create1(0)), events_(epoll_event_size, epoll_event{}) {
        errif(epfd_ == -1, "epoll create error");
    }
    ~Epoll() {
        this->close();
    }

    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;

    Epoll(Epoll&& ep) __THROW : epfd_(ep.release()), events_(std::move(ep.events_)) {}
    Epoll& operator=(Epoll&& ep) {
        epfd_ = ep.release();
        events_ = std::move(ep.events_);
        return *this;
    }

public:
    void updateChannel(Channel* channel);

    void close() {
        if (epfd_ != -1) {
            ::close(epfd_);
            epfd_ = -1;
        }
    }

    int release() {
        int epfd = epfd_;
        epfd_ = -1;
        return epfd;
    }

    std::vector<Channel*> poll(int timeout = -1);
};