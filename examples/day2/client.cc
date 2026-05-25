#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#include "utils.h"

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

    char buf[1024];
    while (true) {
        int num_read = recv(sock_fd, buf, 1024, 0);

        if (num_read > 0) {
            printf("recv server msg: %s", std::string(buf, num_read).c_str());
        } else if (num_read == 0) {
            close(sock_fd);
            break;
        } else if(num_read == -1) {
            close(sock_fd);
            errif(true, "connect always disconnect.");
        }
    }

    return 0;
}
