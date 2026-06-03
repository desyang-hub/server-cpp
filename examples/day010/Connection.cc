#include "Connection.h"

#include <iostream>

#include "utils.h"


Connection::Connection(EventLoop* loop, int fd, bool runInThreadPool) : loop_(loop), ch_(nullptr), sock_(fd) {
    sock_.setnoneblocking();

    ch_ = std::move(std::make_unique<Channel>(loop, sock_.fd(), runInThreadPool));
    ch_->enableRead();
    ch_->enableET();
    ch_->update();
}

void Connection::initReadEventCallBack() {
    // 通过shared_ptr来管理生命周期
    auto self = shared_from_this();
    ch_->setEventCallBack([self]{
        self->echo();
    });
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
        std::cout << "need close" << std::endl;
        if (deleteConnectionCallBack_)
            deleteConnectionCallBack_(sock_.fd());
        else
            std::cout << "error call back" << std::endl;
    }
        
}