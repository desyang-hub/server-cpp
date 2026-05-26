#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#include "utils.h"
#include <thread>
#include <chrono>

#include <string.h>

int main(int argc, char const *argv[])
{
    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    int status = connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr));

    ERRIF(status == -1, "connect error", {close(sock_fd);});

    printf("connected successfully.\n");

    const char* msg = "hello";
    while (true) {
        int num = send(sock_fd, msg, strlen(msg), 0);

        ERRIF(num == -1, "send error", {close(sock_fd);});

        if (num == 0) {
            printf("connect disconnect \n");
            break;
        }

        printf("send msg: %s\n", msg);
        sleep(1);
    }

    return 0;
}
