#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Channel.h"
#include "utils.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "Connection.h"
#include "Buffer.h"

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <memory>

const std::string host = "127.0.0.1";
const int port = 8080;


int main(int argc, char const *argv[])
{
    
    EventLoop loop;
    InetAddress addr(8080);
    TcpServer server(&loop, addr);

    server.setOnConnection([](Connection* conn){
        std::cout << "用户fd=" << conn->fd() << "已经连接到服务器" << std::endl;
    });

    auto echo = [](Connection* conn, Buffer* buf){
        char* readAddr = buf->readerPos();
        int len = buf->readableBytes();
        int num_send = 0;
        try
        {
            num_send = conn->send(readAddr, len);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return;
        }

        if (num_send <= 0) {
            conn->close();
            return;
        }

        buf->retrieve(num_send);
    };

    server.setOnMessageCallBack(echo);

    std::cout << "Epoll single Reactor Lunch, wait user connect..." << std::endl;

    loop.loop();

    return 0;
}
