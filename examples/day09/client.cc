#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

#include "utils.h"

const std::string host = "127.0.0.1";
const int port = 8080;

int main(int argc, char const *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host.c_str());
    server_addr.sin_port = htons(port);

    errif (connect(fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1, "connect error");

    std::cout << "connected to server" << std::endl;
    

    return 0;
}
