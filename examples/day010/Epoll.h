#pragma once

#include <vector>
#include <sys/epoll.h>

#include "nonecopyable.h"

const int EPOLL_EVENT_MAX_SIZE = 1024;

class Channel;

class Epoll : public nonecopyable
{
private:
    int epfd_;
    std::vector<epoll_event> events_;

public:
    explicit Epoll(int size = EPOLL_EVENT_MAX_SIZE);
    ~Epoll();

    int epfd() const;

    void updateChannel(Channel*);

    void removeChannel(int fd);

    void listen_fd(int fd, int ev_option);

    std::vector<Channel*> poll(int timeous=-1);

    void close();
};