#include <iostream>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <functional>

#include "utils.h"
#include <string.h>

#include "socket.h"
#include "Epoll.h"
#include "InetAddress.h"

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
    Socket sock;

    InetAddress server_addr(8080);

    sock.bind(server_addr);

    sock.listen();

    sock.setnoneblocking();

    Epoll epoll;

    epoll.listenfd(sock.fd(), EPOLLIN | EPOLLET);

    while (true) {
        auto events = epoll.poll(-1);

        for (const auto& event : events) {
            if (event.data.fd == sock.fd()) {
                InetAddress client_addr;
                Socket client_sock = sock.accept(client_addr);
                printf("new client: %s\n", client_addr.ipaddr().c_str());

                client_sock.setnoneblocking();

                epoll.listenfd(client_sock.fd(), EPOLLIN | EPOLLET);
            } else if (event.events & EPOLLIN) {
                callBack(event.data.fd);
            }
        }
    }

    return 0;
}
