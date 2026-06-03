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


