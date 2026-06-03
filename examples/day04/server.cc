#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>

const std::string host = "127.0.0.1";
const int port = 8080;


int main(int argc, char const *argv[])
{
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

    // loop accept
    std::cout << "server start, listen port: " << port << std::endl;
    while (true) {
        sockaddr_in cli_addr{};
        socklen_t cli_sock_len = sizeof(cli_addr);
        int cli_fd = accept(fd, (sockaddr*)&cli_addr, &cli_sock_len);


        std::cout << "user fd=" << cli_fd << " connected." << std::endl;
        close(cli_fd);
    }

    return 0;
}
