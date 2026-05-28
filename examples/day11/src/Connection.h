#pragma once

#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <memory>

#include "utils.h"
#include "socket.h"
#include "Channel.h"

class EventLoop;
class Socket;

class Connection : public std::enable_shared_from_this<Connection>
{
    using DeleteConnectionCallBack = std::function<void(int)>;
private:
    Socket sock_;
    EventLoop* loop_;
    ChannelPtr ch_;
    DeleteConnectionCallBack deleteConnectionCallBack_;
    std::string readBuffer_;


public:
    Connection(EventLoop* loop, int fd);
    ~Connection() {
    }

public:
    void deleteConnection() {
        if (deleteConnectionCallBack_) {
            deleteConnectionCallBack_(sock_.fd());
        }
    }

    void initReadEventCallBack();

    void echo();

    void setDeleteConnectionCallBack(DeleteConnectionCallBack cb) {
        deleteConnectionCallBack_ = std::move(cb);
    }
};