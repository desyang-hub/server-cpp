#include "Epoll.h"

#include <unistd.h>

#include "utils.h"

Epoll::Epoll(int size) : epfd_(::epoll_create1(0)), events_(size, epoll_event{}) {
    errif(epfd_ == -1, "epoll create error");
}

Epoll::~Epoll() {
    this->close();
}

int Epoll::epfd() const {
    return epfd_;
}


void Epoll::listen_fd(int fd, int ev_option) {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = ev_option;

    epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
}


std::vector<epoll_event> Epoll::poll(int timeous) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeous);
    errif(nfds == -1, "epoll wait error");

    std::vector<epoll_event> events;
    events.reserve(nfds);

    for (int i = 0; i < nfds; ++i) {
        events.push_back(events_[i]);
    }

    return events;
}

void Epoll::close() {
    if (epfd_ != -1) {
        ::close(epfd_);
        epfd_ = -1;
    }
}