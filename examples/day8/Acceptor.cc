#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Channel.h"
#include "logger.h"

Acceptor::Acceptor(EventLoop* loop, const InetAddress* addr) : loop_(loop), addr_(addr), sock_(), newConnectionCallBack_(nullptr) {
    sock_.bind(*addr);
    sock_.listen(10);

    int fd = sock_.fd();
    Channel* ch = new Channel(loop_, fd, true);

    ch->enableReading();
    ch->setReadEventCallBack(std::bind(&Acceptor::acceptConnection, this));
}

void Acceptor::acceptConnection() {
    InetAddress addr = InetAddress();
    
    Socket sock = sock_.accept(addr);
    sock.setnoneblocking();
    int fd = sock.release();

    LOG_INFO << "user fd " << fd << " " << " ip addr: " << addr << endl;

    Socket* sockPtr = new Socket(fd);

    if (newConnectionCallBack_)
        newConnectionCallBack_(sockPtr);
}

void Acceptor::setNewConnectionCallBack(NewConnectionCallBack cb) {
    newConnectionCallBack_ = std::move(cb);
}