#!/bin/bash
# -DBUILD_SHARED_LIBS=ON 启用动态链接
# -DWITH_EXAMPLES=ON 启用examples构建
cmake -B build  && cmake --build build -j4