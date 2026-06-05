/**
 * @FilePath     : /server-cpp/src/apps/epoll_usage.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:41:08
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "common/utils.h"

const std::string host = "127.0.0.1";
const int port = 8080;
const int DEFAULT_EPOLL_EVENT_SIZE = 1024;


int socket_create() {
    // socket create 
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // bind addr
    sockaddr_in server_addr{}; // 必须进行0初始化，或者使用memset, bzero
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    server_addr.sin_port = htons(port);

    socklen_t socklen = sizeof(server_addr);

    // socket bind
    bind(fd, (sockaddr*)&server_addr, socklen);

    // socket listen
    listen(fd, SOMAXCONN);

    return fd;
}

void event_handler(int fd, int epfd) {
    // LT 模式简单，不必一次将事件全部处理完成

    char buf[1024];

    int nums_read = recv(fd, buf, 1024, 0);
    int errno_num = errno;

    bool need_close = false;

    if (nums_read == -1) {
        if (errno_num == EINTR) {

        } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {

        } else {
            need_close = true;
        }
    } else if (nums_read == 0) {
        need_close = true;
        std::cout << "user=" << fd << " always disconnected." << std::endl;
    } else {
        std::cout << "recv user message: " << std::string(buf, nums_read) << std::endl;
    }

    if (need_close) {
        close(fd);
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
    }
}

void epoll_start() {

    int fd = socket_create();

    // epoll create
    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");

    epoll_event ev{};
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    // epoll ctl add fd to listen
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add main sock error") ;


    // event loop
    std::vector<epoll_event> events(DEFAULT_EPOLL_EVENT_SIZE, epoll_event{});

    std::cout << "epoll server LT mode start, wait user connect..." << std::endl;

    while (true) {
        int nfds = epoll_wait(epfd, events.data(), events.size(), -1);

        errif(nfds == -1, "epoll wait error");

        for (int i = 0; i < nfds; ++i) {
            // 这是一个连接请求事件
            if (events[i].data.fd == fd) {
                sockaddr_in addr{};
                socklen_t socklen = sizeof(addr);

                int cli_fd = accept(fd, (sockaddr*)&addr, &socklen);

                if (cli_fd == -1) {
                    std::cout << "accept error" << std::endl;
                } else {
                    epoll_event ev{};
                    ev.data.fd = cli_fd;
                    ev.events = EPOLLIN;

                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &ev) == -1) {
                        std::cout << "epoll add error" << std::endl;
                        close(cli_fd);
                    } else {
                        std::cout << "user=" << cli_fd << " connected to server" << std::endl;
                    }
                }

            } else { // 这是一个用户请求事件
                event_handler(events[i].data.fd, epfd);
            }
        }
    }

}

int main(int argc, char const *argv[])
{
    
    epoll_start();
    

    return 0;
}
