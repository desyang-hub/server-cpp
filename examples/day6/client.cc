#include "socket.h"
#include "logger.h"

#include <unistd.h>
#include <vector>


int main(int argc, char const *argv[])
{

    InetAddress addr(8080);
    
    Socket sock;

    std::string msg = "hello";

    if (sock.connect(addr)) {
        LOG_INFO << "connect always established." << endl;
        while (true) {
            if (!sock.send(msg)) {
                LOG_ERROR << "send message error" << endl;
                break;
            }

            LOG_INFO << "send msg: " << msg << endl;
            
            sleep(1);
        }
    }

    return 0;
}
