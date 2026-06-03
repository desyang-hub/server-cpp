#pragma once

#include <cstdio>
#include "ServerException.h"

// 满足异常条件condition后，触发退出和msg
inline void errif(bool condition, const char* msg) {
    if (condition) {
        perror(msg);
        throw ServerException(msg);
    }
}