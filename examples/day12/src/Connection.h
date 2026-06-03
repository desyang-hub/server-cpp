#pragma once

#include "Socket.h"
#include "Channel.h"

#include <functional>
#include <memory>

class EventLoop;

class Connection : public std::enable_shared_from_this<Connection>
{
    using HandlerCallBack = std::function<void(Socket*)>;
    using DisconnectCallBack = std::function<void(int)>;
private:
    EventLoop* loop_;
    Socket sock_;
    ChannelPtr channel_;

    HandlerCallBack handlerCallBack_;
    DisconnectCallBack disconnectCallBack_;
public:
    Connection(EventLoop* loop, int fd) : loop_(loop), sock_(fd), channel_(nullptr) {
        sock_.setnoneblocking();

        channel_ = std::make_unique<Channel>(loop_, sock_.fd());
        channel_->enable_reading();
    }

    ~Connection() = default;

public:
    void setHandlerCallBack(const HandlerCallBack& cb) {
        handlerCallBack_ = cb;
    }

    void initEventCallBack() {
        auto self(shared_from_this());
        channel_->set_event_callback([self]{
            if (self->handlerCallBack_) {
                self->handlerCallBack_(&self->sock_);
            }
        });
    }

    void setDisconnectCallBack(const DisconnectCallBack& cb) {
        disconnectCallBack_ = cb;
    }

    void disconnectCallBack() {
        if (disconnectCallBack_) {
            disconnectCallBack_(sock_.fd());
        }
    }
};


using ConnectionPtr = std::shared_ptr<Connection>;