# 2. 重新创建并配置
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 3. 编译
cmake --build build

# 4. 安装
cmake --install build --prefix ./build/tcpserver-install