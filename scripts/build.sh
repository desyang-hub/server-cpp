#!/bin/bash
# -DBUILD_SHARED_LIBS=ON 启用动态链接
cmake -B build && cmake --build build -j4