/**
 * @FilePath     : /server-cpp/src/apps/server.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:41:14
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "common/utils.h"
#include "base/Buffer.h"
#include "net/Socket.h"
#include "net/InetAddress.h"
#include "core/Epoll.h"
#include "core/Channel.h"
#include "core/EventLoop.h"
#include "core/TcpServer.h"
#include "core/Connection.h"

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
