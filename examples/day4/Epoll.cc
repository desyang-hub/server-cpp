#include "Epoll.h"

#include <stdexcept>

#include "utils.h"
#include "logger.h"

Epoll::Epoll(int init_event_size) : epfd_(epoll_create1(0)), events_(init_event_size, epoll_event{})
{
    errif(epfd_ == -1, "epoll create error");
}

Epoll::~Epoll()
{
    this->close();
}


bool Epoll::listenfd(int fd, int event) {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = event;

    return epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) != -1;
}

std::vector<epoll_event> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);
    errif(nfds == -1, "epoll wait error");

    // std::vector<epoll_event> events;
    // events.reserve(nfds);
    // for (int i = 0; i < nfds; ++i) {
    //     events.push_back(events_[i]);
    // }

    return std::vector<epoll_event>(events_.begin(), events_.begin() + nfds);

    // return events;
}