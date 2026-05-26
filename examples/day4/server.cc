#include "Epoll.h"
#include "socket.h"
#include "logger.h"

const int MAX_BUFFER_SIZE = 1024;

void callBack(int fd) {
    printf("fd=%d\n", fd);
    char buf[MAX_BUFFER_SIZE];
    int bytes_read = recv(fd, buf, MAX_BUFFER_SIZE, 0);

    if (bytes_read > 0) {
        printf("recv msg: %s\n", buf);
    } else if (bytes_read == 0) {
        printf("connect disconnect\n");
        close(fd);
    } else {
        printf("recv error");
        close(fd);
    }
}



int main(int argc, char const *argv[])
{

    InetAddress addr(8080);
    
    Socket sock;
    errif(sock.bind(addr) == false, "bind error");
    errif(sock.listen(10) == false, "listen error");

    Epoll epoll;
    errif(epoll.listenfd(sock.fd(), EPOLLIN) == false, "listenfd error");

    LOG_INFO << "Server start" << endl;

    while (true) {
        auto events = epoll.poll(0);

        for (auto& ev : events) {
            if (ev.data.fd == sock.fd()) {
                InetAddress client_addr;
                auto cli_sock = sock.accept(client_addr);
                int fd = cli_sock.release();
                if (fd == -1) {
                    LOG_ERROR << "accept error" << endl;
                    continue;
                }

                bool is_success = epoll.listenfd(fd, EPOLLIN);
                if (!is_success) {
                    LOG_ERROR << "listenfd " << fd << " failed" << endl;
                    close(fd);
                    continue;
                }
            } else {
                // 通过不同的消息，触发不同的事件回调
                callBack(ev.data.fd);
            }
        }
    }


    return 0;
}
