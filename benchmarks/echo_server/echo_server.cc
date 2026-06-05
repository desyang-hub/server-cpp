#include "core/TcpServer.h"
#include "net/InetAddress.h"
#include "core/EventLoop.h"
#include "core/Connection.h"
#include "base/Buffer.h"

int main(int argc, char const *argv[])
{
    
    InetAddress addr(8080);
    EventLoop loop;
    TcpServer server(&loop, addr);

    auto echo = [](Connection* conn, Buffer* buf){
        // echo server 将数据原样回复
        int num = conn->send(buf->readerPos(), buf->readableBytes());
        buf->retrieve(num);
    };

    server.setOnMessageCallBack(echo);

    loop.loop();
    return 0;
}
