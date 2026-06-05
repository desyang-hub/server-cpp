/**
 * @FilePath     : /server-cpp/src/include/core/Epoll.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:39:45
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <vector>
#include <sys/epoll.h>

#include "common/nonecopyable.h"

const int EPOLL_EVENT_MAX_SIZE = 1024;

class Channel;

class Epoll : public nonecopyable
{
private:
    int epfd_;
    std::vector<epoll_event> events_;

public:
    explicit Epoll(int size = EPOLL_EVENT_MAX_SIZE);
    ~Epoll();

    int epfd() const;

    void updateChannel(Channel*);

    void removeChannel(int fd);

    void listen_fd(int fd, int ev_option);

    std::vector<Channel*> poll(int timeous=-1);

    void close();
};