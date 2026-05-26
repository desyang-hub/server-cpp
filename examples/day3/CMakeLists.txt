include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(lib_d3
    Epoll.cc
    socket.cc
)

link_libraries(lib_d3)

add_executable(client_d3 client.cc)
add_executable(epoll_server_d3 epoll_server.cc)
add_executable(epoll_server_d3_et epoll_server_et.cc)