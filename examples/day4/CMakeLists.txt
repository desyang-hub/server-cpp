add_library(lib_d4
    Epoll.cc
    socket.cc
)
link_libraries(lib_d4)


add_executable(client_d4 client.cc)
add_executable(server_d4 server.cc)