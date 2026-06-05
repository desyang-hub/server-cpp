/**
 * @FilePath     : /server-cpp/src/core/Acceptor.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:19
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "core/Acceptor.h"

#include "common/utils.h"

#include <iostream>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr) : sock_(), loop_(loop), chPtr_(std::make_unique<Channel>(loop, sock_.fd())), newConnectionCallBack_(nullptr) {
    sock_.setInetAddrReuse();
    sock_.bind(addr);
    sock_.listen();

    // chPtr_ = ;
    chPtr_->enableRead();
    chPtr_->update();

    chPtr_->setEventCallBack([this]{
        if (newConnectionCallBack_)
            newConnectionCallBack_(&sock_);
    });
}

void Acceptor::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}