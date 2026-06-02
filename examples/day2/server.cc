#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <assert.h>
#include <unistd.h>

#include "utils.h"
#include "socket.h"
#include "ThreadPool.h"

#include <thread>
#include <chrono>
#include <signal.h>

using Seconds = std::chrono::seconds;

void conneted_worker(int fd) {

    Socket sock(fd);

    char buf[1024];
    while (true) {
        int num = sock.recv(buf, 1024);
        if (num == -1) {
            printf("error break\n");
            break;
        } else if (num == 0) {
            printf("connect disconnect\n");
            break;
        } else {
            sock.send(std::string(buf, num));
        }

    }
}


int main(int argc, char const *argv[])
{
    // 忽略 SIGPIPE，让 send() 返回 -1 而不是 kill 进程
    // signal(SIGPIPE, SIG_IGN);

    printf("TcpServer start, wait user connect");
    // 1. create socket
    // int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => IPv4 SOCK_STREAM => 流式 0=> 自动推导TCP还是UDP

    Socket sock;
    errif(!sock.fd() == -1, "socket create error");

    // 2. bind addr
    InetAddress server_addr(8080);
    
    sock.bind(server_addr);

    // 3. 监听请求
    sock.listen();

    ThreadPool pool(4);


    printf("TcpServer start, wait user connect \n");
    while (true) {
        // accept
        // 为接收的连接，准备地址
        InetAddress client_addr;

        int client_fd = sock.accept(client_addr);
        assert(client_fd != -1);
        
        std::cout << "new client connected: " << client_addr << std::endl;

        pool.enqueue(conneted_worker, client_fd);
    }

    return 0;
}
