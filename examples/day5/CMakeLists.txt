add_library(lib_d5
    Epoll.cc
    socket.cc
    Channel.cc
)
link_libraries(lib_d5)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(server_d5
    server.cc
)

add_executable(client_d5
    client.cc
)