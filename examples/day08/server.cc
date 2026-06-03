#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Channel.h"
#include "utils.h"
#include "EventLoop.h"
#include "TcpServer.h"

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <memory>

const std::string host = "127.0.0.1";
const int port = 8080;

void event_handler(int fd, int epfd, std::unordered_map<int, ChannelPtr>& channels) {
    // LT 模式简单，不必一次将事件全部处理完成
    char buf[1024];
    bool need_close = false;
    std::string buffer;

    while (true) {
        int nums_read = recv(fd, buf, 1024, 0);
        int errno_num = errno;
    
        if (nums_read == -1) {
            if (errno_num == EINTR) {
                continue;
            } else if (errno_num == EAGAIN || errno_num == EWOULDBLOCK) {
                send(fd, buffer.data(), buffer.size(), 0);
                break;
            } else {
                need_close = true;
                break;
            }
        } else if (nums_read == 0) {
            need_close = true;
            std::cout << "user=" << fd << " always disconnected." << std::endl;
            break;
        } else {
            buffer.append(buf, nums_read);
            // std::cout << "recv user message: " << std::string(buf, nums_read) << std::endl;
        }
    }

    if (need_close) {
        close(fd);
        // 单线程，不用考虑并发问题
        channels.erase(fd);
    }
}


int main(int argc, char const *argv[])
{
    
    EventLoop loop;
    InetAddress addr(8080);
    TcpServer server(&loop, addr);

    std::cout << "Epoll single Reactor Lunch, wait user connect..." << std::endl;

    loop.loop();

    return 0;
}
