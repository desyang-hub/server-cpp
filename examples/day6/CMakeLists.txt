add_library(lib_d6
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
)
link_libraries(lib_d6)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d6
    server.cc
)

add_executable(client_d6
    client.cc
)