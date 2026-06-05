#pragma once

#include <cstdio>
#include <fcntl.h>
#include "ServerException.h"

// 满足异常条件condition后，触发退出和msg
inline void errif(bool condition, const char* msg) {
    if (condition) {
        perror(msg);
        throw ServerException(msg);
    }
}

// 设置非阻塞IO

inline void setnoneblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get error");
        return;
    }

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}