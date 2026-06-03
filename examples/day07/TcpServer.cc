#include "TcpServer.h"

#include <iostream>

#include "utils.h"


TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr) : loop_(loop), acceptor_(loop, addr) {
    // 多线程可能异常
    if (newConnectionCallBack_) {
        acceptor_.setNewConnectionCallBack([this](Socket* sock){
            newConnectionCallBack_(sock);
        });
    } else {
        acceptor_.setNewConnectionCallBack(std::bind(&TcpServer::newConnectionCallBack, this, std::placeholders::_1));
    }
}

void TcpServer::setNewConnectionCallBack(const NewConnectionCallBack& cb) {
    newConnectionCallBack_ = cb;
}

void TcpServer::newConnectionCallBack(Socket* sock) {
    InetAddress addr;
    int fd = sock->accept(addr);

    {
        Socket sock(fd);
        sock.setnoneblocking();
        sock.release();
    }

    std::cout << "user fd=" << fd << " connected." << std::endl;

    ChannelPtr chanPtr = std::make_shared<Channel>(loop_, fd);
    errif(channels_.count(fd) != 0, "connection always exists");
    channels_[fd] = chanPtr;
    chanPtr->enableRead();
    chanPtr->enableET();
    chanPtr->update();

    chanPtr->setEventCallBack(std::bind(&TcpServer::echo, this, std::placeholders::_1));
}

void TcpServer::setDisConnectionCallBack(const DisConnectionCallBack& cb) {
    disConnectionCallBack_ = cb;
}

void TcpServer::disConnectionCallBack(Socket* sock) {
    if (disConnectionCallBack_) {
        disConnectionCallBack_(sock);
    }
}

void TcpServer::echo(int fd) {
    Socket sock(fd);
    char buf[1024];
    bool need_close = false;
    std::string buffer;

    while (true) {
        int nums_read = sock.recv(buf, 1024);
        int errno_num = errno;
    
        if (nums_read == -1) {
            if (errno_num == EINTR) {
                continue;
            } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {\
                sock.send(buffer.data(), buffer.size());
                break;
            } else {
                need_close = true;
                break;
            }
        } else if (nums_read == 0) {
            need_close = true;
            std::cout << "user=" << sock.fd() << " always disconnected." << std::endl;
            break;
        } else {
            buffer.append(buf, nums_read);
            // std::cout << "recv user message: " << std::string(buf, nums_read) << std::endl;
        }
    }

    if (need_close) {
        // 单线程，不用考虑并发问题
        channels_.erase(sock.fd());
        sock.close();
    } else {
        sock.release();
    }
}