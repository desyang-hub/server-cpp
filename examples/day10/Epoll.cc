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

std::vector<Channel*> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_.data(), events_.size(), timeout);

    
    if (nfds == -1) {
        if (errno == EINTR)
            return {};

        errif(nfds == -1, "epoll wait error");
    }

    std::vector<Channel*> activeChannels;
    activeChannels.reserve(nfds);
    

    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (int i = 0; i < nfds; ++i) {
            Channel* ch = (Channel*)events_[i].data.ptr;
            activeChannels.push_back(ch);
        }
    }

    return activeChannels;
}

// 对于一个已经断开连接的fd, 由于我们将其绑定到channelMap中，所有必须要对断开的资源进行释放，否则会造成内存泄漏
bool Epoll::updateChannel(Channel* ch) {
    if (!ch) return true;

    int fd = ch->fd();
    epoll_event ev{};
    ev.data.ptr = ch;
    ev.events = ch->events();

    int state;

    // 如果已经在里面了用EPOLL_CTL_MOD
    if (ch->isInEpoll()) {
        state = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
    } else {
        ch->setIsInEpoll(true);
        state = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
    }

    if (state == -1) {
        perror("epoll ctl error");
        LOG_ERROR << "Epoll::updateChannel error" << endl;
    }

    return state != -1;
}

void Epoll::removeChannel(ChannelPtr ch) {
    int fd = ch->fd();
    epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
}