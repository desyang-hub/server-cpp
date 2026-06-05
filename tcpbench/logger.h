/**
 * @FilePath     : /server-cpp/test/logger.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 14:42:19
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 14:42:20
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <iostream>

using std::endl;

#define LOG_INFO std::cout << "[INFO] "
#define LOG_WARN std::cout << "[WARNNING] "
#define LOG_ERROR std::cerr << "[ERROR] "

#define LOG_FATAL(format, ...) \
    printf("%s:%d", __FILE__, __LINE__); \
    printf(format, ##__VA_ARGS__); \
    exit(-1);


