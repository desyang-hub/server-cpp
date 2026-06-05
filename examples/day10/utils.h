/**
 * @FilePath     : /server-cpp/examples/day10/utils.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:03:51
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 15:03:53
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <cassert>

using Seconds = std::chrono::seconds;

#define ERRIF(condition, msg, expr) \
    if (condition) { perror(msg); {expr} exit(-1); }

inline void Sleep(size_t seconds) {
    std::this_thread::sleep_for(Seconds(seconds));
}
    
inline void errif(bool condition, const char* msg) {
    if (condition) {
        perror(msg);
        exit(-1);
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

#define Assert(expr) assert(expr)