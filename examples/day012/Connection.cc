#include "Connection.h"

#include <iostream>
#include <sys/epoll.h>

#include "utils.h"

/// @brief 用于触发事件回调过程中，将消息读取到buffer中
void Connection::handelEvent() {
    int n = readBuffer_.readFd(sock_.fd());
    if (n <= 0) {
        this->close();
    }

    // bool isEtMode = ch_.events() & EPOLLET;
    // char buf[1024];
    // bool need_close = false;

    // do {
    //     int nums_read = sock_.recv(buf, 1024);
    //     int errno_num = errno;
    
    //     if (nums_read == -1) {
    //         if (errno_num == EINTR) {
    //             continue;
    //         } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
    //             break;
    //         } else {
    //             need_close = true;
    //             break;
    //         }
    //     } else if (nums_read == 0) {
    //         need_close = true;
    //         break;
    //     } else {
    //         readBuffer_.append(buf, nums_read);
    //         // std::cout << "recv user message: " << std::string(buf, nums_read) << std::endl;
    //     }
    // } while (isEtMode);

    // if (need_close) {
    //     this->close();
    // }
}


Connection::Connection(EventLoop* loop, int fd, bool runInThreadPool) : loop_(loop), sock_(fd), ch_(loop, sock_.fd(), runInThreadPool), readBuffer_(), deleteConnectionCallBack_(nullptr), onMessageCallBack_(nullptr) {
    sock_.setnoneblocking();

    // ch_ = std::move(std::make_unique<Channel>(loop, sock_.fd(), runInThreadPool));
    ch_.enableRead();
    ch_.enableET();
    ch_.update();
}

int Connection::fd() const {
    return sock_.fd();
}

void Connection::initReadEventCallBack() {
    // 通过shared_ptr来管理生命周期
    // auto self = 
    // auto self =  shared_from_this(); 原先用，导致所有对象不析构
    std::weak_ptr<Connection> wk_self = shared_from_this();
    ch_.setEventCallBack([wk_self]{
        auto self = wk_self.lock();
        if (self) {
            self->handelEvent();
            if (self->onMessageCallBack_) {
                self->onMessageCallBack_(self.get(), &self->readBuffer_);
            }
        }
        // self->echo();
    });
}

void Connection::setDeleteConnectionCallBack(const DeleteConnectionCallBack& cb) {
    deleteConnectionCallBack_ = cb;
}

void Connection::setOnMessageCallBack(const OnMessageCallBack& cb) {
    onMessageCallBack_ = cb;
}

void Connection::close() {
    if (deleteConnectionCallBack_)
        deleteConnectionCallBack_(sock_.fd());
}

/// @brief 发送数据到对端
/// @param msg 数据
/// @return 
int Connection::send(const std::string& msg) {
    return this->send(msg.c_str(), msg.size());
}

int Connection::send(const char* buf, size_t len) {
    return sock_.send(buf, len);
}