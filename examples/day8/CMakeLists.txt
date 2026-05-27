add_library(lib_d8
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
    Acceptor.cc
    Connection.cc
)
link_libraries(lib_d8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d8
    server.cc
)

add_executable(client_d8
    client.cc
)