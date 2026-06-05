/**
 * @FilePath     : /server-cpp/src/core/Epoll.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:31
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "core/Epoll.h"

#include <unistd.h>

#include "common/utils.h"
#include "core/Channel.h"

Epoll::Epoll(int size) : epfd_(::epoll_create1(0)), events_(size, epoll_event{}) {
    errif(epfd_ == -1, "epoll create error");
}

Epoll::~Epoll() {
    this->close();
}

int Epoll::epfd() const {
    return epfd_;
}

void Epoll::updateChannel(Channel* ch) {
    epoll_event ev{};
    ev.data.ptr = ch;
    ev.events = ch->events();

    // channel alwas register to epoll, just mod
    if (ch->isInEpoll()) {
        errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, ch->fd(), &ev) == -1, "epoll mod error");
    } else {
        ch->setInEpoll();
        errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, ch->fd(), &ev) == -1, "epoll add error");
    }
}

void Epoll::removeChannel(int fd) {
    epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
}


void Epoll::listen_fd(int fd, int ev_option) {
    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = ev_option;

    epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
}


std::vector<Channel*> Epoll::poll(int timeous) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeous);
    std::vector<Channel*> events;
    if (nfds == -1) {
        if (errno == EINTR) {
            return events;
        }
        
        errif(true, "epoll wait error");
    }
    
    events.reserve(nfds);

    Channel* ch{};
    for (int i = 0; i < nfds; ++i) {
        ch = (Channel*)events_[i].data.ptr;
        ch->setRevents(events_[i].events);
        events.push_back(ch);
    }

    return events;
}

void Epoll::close() {
    if (epfd_ != -1) {
        ::close(epfd_);
        epfd_ = -1;
    }
}