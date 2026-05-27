#include "TcpServer.h"

#include <functional>
#include <cerrno>

#include "EventLoop.h"
#include "socket.h"
#include "Channel.h"
#include "logger.h"

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : loop_(loop), acceptor_(loop_, &addr) {
    // 将socket建立出来，并绑定事件后通过loop注册到epoll
    acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
}

void TcpServer::messageCallBack(Channel* ch) {
    int fd = ch->fd();
    Socket sock(fd);

    char buf[1024];

    while (true) {
        int num_read = sock.recv(buf, 1024);

        int errno_num = errno;

        if (num_read > 0) {
            LOG_INFO << "recv message: " << std::string(buf, num_read) << endl;
        } else if (num_read == 0) {
            LOG_INFO << "connect always disconnect" << endl;
            ch->remove();
            // 连接断开移除
            break;
        } else {
            if (errno_num == EINTR) {
                continue;
            } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
                break;
            } else {
                LOG_ERROR << "read message error" << endl;
                ch->remove();
                break;
            }
        }
    }

    sock.release();
}

void TcpServer::newConnection(int fd) {
    Channel* ch = new Channel(loop_, fd);
    ch->enableReading();
    ch->setReadEventCallBack(std::bind(&TcpServer::messageCallBack, this, std::placeholders::_1));
}