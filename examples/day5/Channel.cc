#include "Channel.h"

#include "Epoll.h"

bool Channel::update() {
    return epoll_->updateChannel(this);
}