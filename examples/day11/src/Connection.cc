#include "Connection.h"

#include <errno.h>
#include <string.h>

#include "EventLoop.h"
#include "Channel.h"
#include "socket.h"
#include "logger.h"

Connection::Connection(EventLoop* loop, int fd) : loop_(loop), sock_(fd), ch_(std::make_shared<Channel>(loop_, fd)) {
    sock_.setnoneblocking();
    ch_->enableReading();
}

void Connection::initReadEventCallBack() {
    auto self = shared_from_this();

    ch_->setReadEventCallBack([self]{
        self->echo();
    });
}

void Connection::echo() {
    // 收到消息后转发回去
    char buf[1024];
    while (true) {
        int bytes_read = sock_.recv(buf, 1024);
        int errno_num = errno;

        if (bytes_read == -1) {
            if (errno_num == EINTR) {
                continue;
            }

            if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
                sock_.send(readBuffer_);
                readBuffer_.clear();
                break;
            }

            LOG_ERROR << "fd " << sock_.fd() <<  " echo error: " << strerror(errno) << endl;
            readBuffer_.clear();
            deleteConnection(); // 删除连接
            return;
        } else if (bytes_read == 0) {
            // LOG_INFO << "fd " << sock_.fd() <<  " disconnect. " << endl;
            readBuffer_.clear();
            deleteConnection(); // 删除连接
            return;
        } else {
            readBuffer_.append(buf, bytes_read);
        }
    }
}