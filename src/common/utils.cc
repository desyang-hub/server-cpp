/**
 * @FilePath     : /server-cpp/src/common/utils.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 16:28:02
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:49
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "common/utils.h"

// 满足异常条件condition后，触发退出和msg
void errif(bool condition, const char* msg) {
    if (condition) {
        perror(msg);
        throw ServerException(msg);
    }
}

// 设置非阻塞IO

void setnoneblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get error");
        return;
    }

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}