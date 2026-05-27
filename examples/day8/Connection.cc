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
    int bytes_read = sock_->recv(buf, 1024);

    if (bytes_read == -1) {
        LOG_ERROR << "fd " << sock_->fd() <<  " echo error: " << strerror(errno) << endl;
        deleteConnection(); // 删除连接
    } else if (bytes_read == 0) {
        LOG_INFO << "fd " << sock_->fd() <<  " disconnect. " << endl;
        deleteConnection(); // 删除连接
    } else {
        sock_->send(std::string(buf, bytes_read));
    }
}