add_library(lib_d7
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
    Acceptor.cc
)
link_libraries(lib_d7)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d7
    server.cc
)

add_executable(client_d7
    client.cc
)