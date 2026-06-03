#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Channel.h"
#include "utils.h"

#include <iostream>
#include <unistd.h>
#include <unordered_map>
#include <memory>

using ChannelPtr = std::unique_ptr<Channel>;

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
    // socket create 
    Socket sock;

    // bind addr
    InetAddress addr(port, host.c_str());

    // socket bind
    sock.bind(addr);

    // sock.setInetAddrReuse();
    sock.setnoneblocking();

    // socket listen
    sock.listen();

    // epoll create
    Epoll epoll;

    // epoll.listen_fd(sock.fd(), EPOLLIN);

    std::unordered_map<int, ChannelPtr> channels;
    channels[sock.fd()] = std::move(std::make_unique<Channel>(&epoll, sock.fd()));
    channels[sock.fd()]->enableRead();
    channels[sock.fd()]->update();


    // TODO:  这正是 ET 模式 + listen fd 的经典陷阱！
    // 若使用 ET（Edge Triggered）模式：
    // 必须一次性 accept 所有连接，否则：
    // 如果只 accept 一次，而 backlog 中还有多个连接，
    // 后续 epoll_wait 不会再通知你（因为 ET 只在状态变化时触发一次），
    // 导致连接堆积甚至超时。

    // loop accept
    std::cout << "Epoll ET server start, listen port: " << port << std::endl;
    while (true) {
        std::vector<Channel*> activeChannels = epoll.poll(-1);

        for (auto& ch : activeChannels) {
            if (ch->fd() == sock.fd()) {
                // mainReactor 使用的是LT触发，不必一次处理所有的accept
                InetAddress cli_addr;
                int fd = sock.accept(cli_addr);

                Socket cli_sock(fd);
                cli_sock.setnoneblocking();

                fd = cli_sock.release();
                
                errif (channels.count(fd) != 0, "fd always exists.");
                // epoll.listen_fd(cli_sock.release(), EPOLLIN | EPOLLET);
                channels[fd] = std::move(std::make_unique<Channel>(&epoll, fd));
                channels[fd]->enableRead();
                channels[fd]->enableET();
                channels[fd]->update();

                std::cout << "user addr: " << cli_addr.toIpPort() << std::endl;
            } else {
                event_handler(ch->fd(), epoll.epfd(), channels);
            }
        }
    }

    return 0;
}
