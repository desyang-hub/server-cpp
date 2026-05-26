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

    // 将所有的channel* delete
    for (auto& p : channelMap_) {
        delete p.second;
    }
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

// 对于一个已经断开连接的fd, 由于我们将其绑定到channelMap中，所有必须要对断开的资源进行释放，否则会造成内存泄漏
bool Epoll::updateChannel(Channel* channel) {
    if (!channel) return true;

    channelMap_[channel->fd()] = channel;

    epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->events();

    int state;

    // 如果已经在里面了用EPOLL_CTL_MOD
    if (channel->isInEpoll()) {
        state = epoll_ctl(epfd_, EPOLL_CTL_MOD, channel->fd(), &ev);
    } else {
        channel->setIsInEpoll(true);
        state = epoll_ctl(epfd_, EPOLL_CTL_ADD, channel->fd(), &ev);
    }

    if (state == -1) {
        perror("epoll ctl error");
        LOG_ERROR << "Epoll::updateChannel error" << endl;
    }

    return state != -1;
}

void Epoll::removeChannel(Channel* ch) {
    int fd = ch->fd();
    epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);

    delete channelMap_[fd];
    channelMap_.erase(fd);
}