#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr));

    printf("connected successfully.\n");

    return 0;
}
