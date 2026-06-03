include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${LIB_NAME}
    Epoll.cc
    Channel.cc
    TcpServer.cc
)
