#pragma once

#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "EventLoop.h"

#include <iostream>
#include <cassert>

class Acceptor
{
    using NewConnectionCallBack = std::function<void(int)>;
private:
    EventLoop* loop_;
    Socket sock_;
    ChannelPtr ch_;

    NewConnectionCallBack new_connection_call_back_;
public:
    Acceptor(EventLoop* loop, const InetAddress& listen_addr) : sock_(), loop_(loop), ch_(nullptr) {
        sock_.bind(listen_addr);
        sock_.listen();

        // 这里使用LT模式监听新连接，acceptor不需要设置成非阻塞
        ch_ = std::make_unique<Channel>(loop_, sock_.fd());
        ch_->set_events(EPOLLIN);
        ch_->update();
        ch_->set_event_callback(std::bind(&Acceptor::handleNewConnection, this));
    }
    ~Acceptor() = default;

public:
    void handleNewConnection() {
        InetAddress peer_addr;
        int peer_fd = sock_.accept(peer_addr);

        assert(peer_fd != -1);

        std::cout << "New connection from " << peer_addr.toIpPort() << std::endl;

        if (new_connection_call_back_) {
            new_connection_call_back_(peer_fd);
        }
    }

    void setNewConnectionCallBack(const NewConnectionCallBack& cb) {
        new_connection_call_back_ = cb;
    }
};