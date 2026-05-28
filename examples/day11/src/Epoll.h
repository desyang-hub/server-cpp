#pragma once

#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <Channel.h>

namespace
{
    const int DEFAULT_EPOLL_EVENTS = 1024;
} // namespace name


class Epoll
{
private:
    int epfd_;
    std::vector<epoll_event> events_;
    mutable std::mutex mutex_;

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

    std::vector<Channel*> poll(int timeout = -1);

    bool updateChannel(Channel* ch);

    void removeChannel(ChannelPtr ch);
};

