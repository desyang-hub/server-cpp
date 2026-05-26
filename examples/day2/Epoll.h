#pragma once

#include <sys/epoll.h>
#include <vector>
#include <unistd.h>

namespace
{
    const int DEFAULT_EPOLL_EVENTS = 1024;
} // namespace name


class Epoll
{
private:
    int epfd_;
    std::vector<epoll_event> events_;
public:
    Epoll(int init_event_size = DEFAULT_EPOLL_EVENTS);
    ~Epoll();

public:
    void close() {
        if (epfd_ != -1) {
            ::close(epfd_);
            epfd_ = -1;
        }
    }

    // event => EPOLLIN | EPOLLET
    bool listenfd(int fd, int event);

    std::vector<epoll_event> poll(int timeout = -1);
};

