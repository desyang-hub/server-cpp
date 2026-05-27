add_library(lib_d9
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
    Acceptor.cc
    Connection.cc
)
link_libraries(lib_d9)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d9
    server.cc
)

add_executable(client_d9
    client.cc
)