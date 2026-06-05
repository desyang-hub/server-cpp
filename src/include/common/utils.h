/**
 * @FilePath     : /server-cpp/src/include/common/utils.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:09
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <cstdio>
#include <fcntl.h>
#include "common/ServerException.h"

/// @brief 满足条件condition触发异常抛出
/// @param condition 条件变量
/// @param msg 异常信息
void errif(bool condition, const char* msg);

// 设置非阻塞IO
void setnoneblocking(int fd);