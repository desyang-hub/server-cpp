#include "Channel.h"

#include "Epoll.h"

void Channel::update() {
    epoll_->updateChannel(this);
}