#include "Channel.h"

#include "EventLoop.h"

void Channel::update() {
    loop_->updateChannel(this);
}