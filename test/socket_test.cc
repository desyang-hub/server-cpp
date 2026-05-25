#include <socket.h>
#include <thread>
#include <chrono>
#include <stdint.h>

const int port = 8080;
const std::string host = "127.0.0.1";

using Seconds = std::chrono::seconds;
void sleep(int seconds);
void server_worker();
void client_worker();

int main(int argc, char const *argv[])
{
    std::thread server(server_worker);
    sleep(1);
    std::thread client(client_worker);
    server.join();
    client.join();

    return 0;
}

void sleep(int seconds) {
    std::this_thread::sleep_for(Seconds(seconds));
}

void server_worker() {
    Socket sock;
    sock.bind(port);
    sock.listen(3);
    int fd = sock.accept();
    printf("fd %d connected success.\n", fd);
}

void client_worker() {
    Socket sock;
    bool is_success = sock.connect(port);
    if (is_success) {
        printf("connected to server succesfully.\n");
    } else {
        printf("connected failed.\n");
    }
}