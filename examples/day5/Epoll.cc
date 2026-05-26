#include "Epoll.h"

#include <stdexcept>

#include "utils.h"
#include "logger.h"
#include "Channel.h"

Epoll::Epoll(int init_event_size) : epfd_(epoll_create1(0)), events_(init_event_size, epoll_event{})
{
    errif(epfd_ == -1, "epoll create error");
}

Epoll::~Epoll()
{
    this->close();
}


bool Epoll::listenfd(int fd, int event) {
    Channel* channel = new Channel(this, fd);
    channel->setEvents(event);
    
    channelMap_[fd] = channel;
    return channel->update();
}

// std::vector<epoll_event> Epoll::poll(int timeout) {
//     int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);
//     errif(nfds == -1, "epoll wait error");

//     // std::vector<epoll_event> events;
//     // events.reserve(nfds);
//     // for (int i = 0; i < nfds; ++i) {
//     //     events.push_back(events_[i]);
//     // }

//     return std::vector<epoll_event>(events_.begin(), events_.begin() + nfds);

//     // return events;
// }

std::vector<Channel*> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);

    errif(nfds == -1, "epoll wait error");

    std::vector<Channel*> activeChannels;
    activeChannels.reserve(nfds);

    for (int i = 0; i < nfds; ++i) {
        activeChannels.push_back((Channel*)events_[i].data.ptr);
    }

    return activeChannels;
}

bool Epoll::updateChannel(Channel* channel) {
    if (!channel) return true;

    epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->events();

    int state;

    // 如果已经在里面了用EPOLL_CTL_MOD
    if (channel->isInEpoll()) {
        state = epoll_ctl(epfd_, EPOLL_CTL_MOD, channel->fd(), &ev);
    } else {
        state = epoll_ctl(epfd_, EPOLL_CTL_ADD, channel->fd(), &ev);
    }

    if (state == -1) {
        LOG_ERROR << "Epoll::updateChannel error" << endl;
    }

    return state != -1;
}