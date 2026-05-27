add_library(lib_d10
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
    Acceptor.cc
    Connection.cc
)
link_libraries(lib_d10)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d10
    server.cc
)

add_executable(client_d10
    client.cc
)

add_executable(ThreadPoolTest
    ThreadPoolTest.cc
)