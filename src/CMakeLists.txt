set(TCPSERVER tcpserver)

add_library(${TCPSERVER}
    Acceptor.cc
    Buffer.cc
    Channel.cc
    client.cc
    CMakeLists.txt
    Connection.cc
    epoll_usage.cc
    Epoll.cc
    EventLoop.cc
    InetAddress.cc
    server.cc
    Socket.cc
    TcpServer.cc
    test.cc
    ThreadPool.cc
)

target_include_directories(${TCPSERVER} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 发布模式设置优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # 或多配置生成器（如 Visual Studio）
    target_compile_options(mylib PRIVATE $<$<CONFIG:Release>:-O3>)
endif()