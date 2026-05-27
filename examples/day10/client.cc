#include "socket.h"
#include "logger.h"

#include <unistd.h>
#include <vector>


int main(int argc, char const *argv[])
{

    InetAddress addr(8080);
    
    Socket sock;

    std::string msg = "hello";

    char buf[1024];

    if (sock.connect(addr)) {
        LOG_INFO << "connect always established." << endl;
        while (true) {
            if (!sock.send(msg)) {
                LOG_ERROR << "send message error" << endl;
                break;
            }
            LOG_INFO << "send msg: " << msg << endl;

            int bytes_read = sock.recv(buf, 1024);
            errif(bytes_read == -1, "recv error");

            if (bytes_read == 0) {
                LOG_INFO << "connect always disconnect. " << endl;
                break;
            }

            LOG_INFO << "recv msg: " << std::string(buf, bytes_read) << endl;
            
            sleep(1);
        }
    }

    return 0;
}
