#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>


int main(int argc, char const *argv[])
{
    // 1. create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET => IPv4 SOCK_STREAM => 流式 0=> 自动推导TCP还是UDP

    // 2. bind addr
    sockaddr_in server_addr{}; // 自动初始化

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    
    bind(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr));

    // 3. 监听请求
    listen(sock_fd, 3);

    // accept

    // 为接收的连接，准备地址
    sockaddr_in client_addr{};
    socklen_t sock_len = sizeof(client_addr);
    int client_fd = accept(sock_fd, (sockaddr*)&client_addr, &sock_len);

    // 连接成功打印地址
    printf("client fd[%d] addr: %s:%d \n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));\
    return 0;
}
