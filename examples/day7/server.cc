#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"


int main(int argc, char const *argv[])
{

    InetAddress addr(8080);
    EventLoop loop;
    TcpServer server(&loop, addr);

    loop.loop();

    return 0;
}
