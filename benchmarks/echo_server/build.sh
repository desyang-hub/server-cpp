cmake -B build && cmake --build build -j4

# 动态链接，若库未安装到系统目录，则需要设置链接地址
# export LD_LIBRARY_PATH="/home/desyang/github/server-cpp/build/tcpserver-install/lib:$LD_LIBRARY_PATH"