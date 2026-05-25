#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define Throw

#define ERRIF(condition, msg, expr) \
    if (condition) { perror(msg); {expr} exit(-1); }

inline void errif(bool condition, const char* msg = "") {
    if (condition) {
        perror(msg);
        exit(-1);
    }
}