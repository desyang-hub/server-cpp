include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${LIB_NAME}
    Epoll.cc
    socket.cc
    Channel.cc
    EventLoop.cc
    TcpServer.cc
    Acceptor.cc
    Connection.cc
)
