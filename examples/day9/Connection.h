#pragma once

#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <string>

#include "utils.h"
#include "socket.h"

class EventLoop;
class Socket;

class Connection
{
    using DeleteConnectionCallBack = std::function<void(Socket*)>;
private:
    Socket* sock_;
    EventLoop* loop_;
    DeleteConnectionCallBack deleteConnectionCallBack_;
    std::string readBuffer_;


public:
    Connection(EventLoop* loop, Socket* sock);
    ~Connection() {
        if (sock_) {
            delete sock_;
            sock_ = nullptr;
        }
    }

public:
    void deleteConnection() {
        if (deleteConnectionCallBack_) {
            deleteConnectionCallBack_(sock_);
        }
    }

    void echo();

    void setDeleteConnectionCallBack(DeleteConnectionCallBack cb) {
        deleteConnectionCallBack_ = std::move(cb);
    }
};