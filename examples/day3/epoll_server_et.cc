#include <iostream>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <functional>

#include <utils.h>
#include <string.h>

const int MAX_EVENTS = 1024;
const int MAX_BUFFER_SIZE = 1024;

void callBack(int fd) {
    printf("fd=%d\n", fd);

    char buf[MAX_BUFFER_SIZE];

    while (true) {
        int bytes_read = recv(fd, buf, MAX_BUFFER_SIZE, 0);

        int errnum = errno;

        if (bytes_read > 0) {
            printf("recv msg: %s\n", buf);
        } else if (bytes_read == 0) {
            printf("connect disconnect\n");
            break;
        } else {
            if (errnum == EINTR) { // 正常中断，继续读取
                continue;
            } else if (errnum == EAGAIN || errnum == EWOULDBLOCK) { // 数据读取完成
                break;
            } else { // discover exception
                printf("recv error: %s\n", strerror(errnum));
                close(fd);
                break;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    // 1. 创建sock
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    errif(sock_fd == -1, "socket create error");

    setnoneblocking(sock_fd);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    ERRIF(bind(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1,
        "bind error", 
        {close(sock_fd);});

    ERRIF(listen(sock_fd, 3) == -1, "listen error", {close(sock_fd);});

    
    // 2. 创建epoll_fd
    int epfd = epoll_create1(0);

    epoll_event ev{};
    ev.data.fd = sock_fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev);

    epoll_event events[MAX_EVENTS];

    // 默认LT触发，水平触发，只有还有消息就一直发送

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, 0);

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sock_fd) {

                sockaddr_in addr{};
                socklen_t sockaddr_len = sizeof(addr);

                int fd = accept(sock_fd, (sockaddr*)&addr, &sockaddr_len);
                if (fd != -1) {
                    ev.data.fd = fd;
                    ev.events = EPOLLIN | EPOLLET;
                }

                setnoneblocking(fd);

                if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
                    close(fd);
                    printf("epoll add sockfd error %d\n", fd);
                    continue;
                }
                
            } else {
                if (events[i].events & EPOLLIN || events[i].events & EPOLLET) { // 调用对应的事件
                    callBack(events[i].data.fd);
                }
            }
        }
    }

    return 0;
}
