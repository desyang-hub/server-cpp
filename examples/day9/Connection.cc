#include "Connection.h"

#include <errno.h>
#include <string.h>

#include "EventLoop.h"
#include "Channel.h"
#include "socket.h"
#include "logger.h"

Connection::Connection(EventLoop* loop, Socket* sock) : loop_(loop), sock_(sock) {
    int fd = sock_->fd();
    Channel* ch = new Channel(loop_, fd);
    ch->enableReading();
    ch->setReadEventCallBack(std::bind(&Connection::echo, this));
}

void Connection::echo() {
    // 收到消息后转发回去
    char buf[1024];

    while (true) {
        int bytes_read = sock_->recv(buf, 1024);
        int errno_num = errno;

        if (bytes_read == -1) {
            if (errno_num == EINTR) {
                continue;
            }

            if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
                break;
            }

            LOG_ERROR << "fd " << sock_->fd() <<  " echo error: " << strerror(errno) << endl;
            readBuffer_.clear();
            deleteConnection(); // 删除连接
            return;
        } else if (bytes_read == 0) {
            LOG_INFO << "fd " << sock_->fd() <<  " disconnect. " << endl;
            readBuffer_.clear();
            deleteConnection(); // 删除连接
            return;
        } else {
            readBuffer_.append(buf, bytes_read);
        }
    }

    // 读取完一个轮回或者时正常断开连接时，将数据发送出去
    sock_->send(readBuffer_);
    readBuffer_.clear();
}