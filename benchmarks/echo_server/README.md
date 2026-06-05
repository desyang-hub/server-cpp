# 使用须知

构建之前修改CMakeLists.txt文件中设置路径告诉 CMake 去哪里找 TCPServer
list(APPEND CMAKE_PREFIX_PATH "path/install/")

如果安装到了系统路径就可以忽略