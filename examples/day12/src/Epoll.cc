#include "Epoll.h"

#include "Channel.h"
#include "utils.h"

void Epoll::updateChannel(Channel* channel) {

    epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->events();

    if (channel->is_in_epoll()) {
        Assert(epoll_ctl(epfd_, EPOLL_CTL_MOD, channel->fd(), &ev) != -1 && "epoll mod error");
    } else {
        Assert(epoll_ctl(epfd_, EPOLL_CTL_ADD, channel->fd(), &ev) != -1 && "epoll add error");
        channel->set_in_epoll();
    }
}

void Epoll::removeChannel(Channel* channel) {
    Assert(epoll_ctl(epfd_, EPOLL_CTL_DEL, channel->fd(), nullptr) != -1 && "epoll delete error");
}

std::vector<Channel*> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);
    errif(nfds == -1, "epoll wait error");

    std::vector<Channel*> activeChannels;
    activeChannels.reserve(nfds);

    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)events_[i].data.ptr;
        ch->set_revents(events_[i].events);
        activeChannels.push_back(ch);
    }

    return activeChannels;
}