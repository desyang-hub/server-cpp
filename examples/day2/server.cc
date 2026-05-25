#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>

#include "utils.h"

#include <thread>
#include <chrono>
#include <signal.h>

using Seconds = std::chrono::seconds;

void conneted_worker(int fd) {
    while (true) {
        char buf[1024];
        int bytes_num = snprintf(buf, 1024, "wellcome! we will improve server for your.\n");

        int num;
        num = send(fd, buf, bytes_num, 0);

        if (num == -1) {
            printf("error break\n");
            break;
        } else if (num == 0) {
            printf("connect disconnect\n");
            break;
        }

        printf("sleep\n");
        std::this_thread::sleep_for(Seconds(1));
    }
}


int main(int argc, char const *argv[])
{
    // 忽略 SIGPIPE，让 send() 返回 -1 而不是 kill 进程
    signal(SIGPIPE, SIG_IGN);

    printf("Server start, wait user connect");
    // 1. create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => IPv4 SOCK_STREAM => 流式 0=> 自动推导TCP还是UDP

    errif(sock_fd == -1, "socket create error");

    // 2. bind addr
    sockaddr_in server_addr{}; // 自动初始化

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    
    int len = bind(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr));

    ERRIF(len == -1, "bind error", {close(sock_fd);});
    

    // 3. 监听请求
    int status = listen(sock_fd, 3);

    ERRIF(status == -1, "listen error", {close(sock_fd);});


    printf("Server start, wait user connect \n");
    while (true) {
        // accept
        // 为接收的连接，准备地址
        sockaddr_in client_addr{};
        socklen_t sock_len = sizeof(client_addr);


        int client_fd = accept(sock_fd, (sockaddr*)&client_addr, &sock_len);

        ERRIF(client_fd == -1, "accept error", {close(sock_fd);});

        // 连接成功打印地址
        printf("client fd[%d] addr: %s:%d \n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        std::thread th(conneted_worker, client_fd);
        th.detach();
    }

    return 0;
}
