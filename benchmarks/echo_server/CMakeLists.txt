# CMakeLists.txt for echo_server
cmake_minimum_required(VERSION 3.16)
project(echo_server)

# 告诉 CMake 去哪里找 TCPServer
list(APPEND CMAKE_PREFIX_PATH "/home/desyang/github/server-cpp/build/tcpserver-install")

find_package(TCPServer REQUIRED)

add_executable(echo_server echo_server.cc)

# 只需这一行！自动传递所有依赖
target_link_libraries(echo_server PRIVATE TCPServer::tcp_core)