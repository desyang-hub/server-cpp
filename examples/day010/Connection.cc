#include "Connection.h"

#include <iostream>

#include "utils.h"


Connection::Connection(EventLoop* loop, int fd) : loop_(loop), ch_(nullptr), sock_(fd) {
    sock_.setnoneblocking();

    ch_ = std::move(std::make_unique<Channel>(loop, sock_.fd()));
    ch_->enableRead();
    ch_->enableET();
    ch_->update();

    // 多线程下，可能会有异常，因为ch被注册到epoll中，此时这里的this，可能已经被别的线程delete了，所以并发场景需要通过shared_ptr来解决这个问题，确保保留一份实例
    ch_->setEventCallBack(std::bind(&Connection::echo, this));
}

void Connection::setDeleteConnectionCallBack(const DeleteConnectionCallBack& cb) {
    deleteConnectionCallBack_ = cb;
}

void Connection::echo() {
    char buf[1024];
    bool need_close = false;

    while (true) {
        int nums_read = sock_.recv(buf, 1024);
        int errno_num = errno;
    
        if (nums_read == -1) {
            if (errno_num == EINTR) {
                continue;
            } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
                sock_.send(readBuffer_.retrieveAllAsString());
                break;
            } else {
                need_close = true;
                break;
            }
        } else if (nums_read == 0) {
            need_close = true;
            std::cout << "user=" << sock_.fd() << " always disconnected." << std::endl;
            break;
        } else {
            readBuffer_.append(buf, nums_read);
            // std::cout << "recv user message: " << std::string(buf, nums_read) << std::endl;
        }
    }

    if (need_close) {
        errif (!deleteConnectionCallBack_, "deleteConnectionCallBack_ is unset");
        
        if (deleteConnectionCallBack_)
            deleteConnectionCallBack_(&sock_);
        else
            std::cout << "error call back" << std::endl;
    }
        
}