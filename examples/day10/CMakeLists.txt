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

# 启用 AddressSanitizer
target_compile_options(server_d10 PRIVATE -fsanitize=address -fno-omit-frame-pointer -O1)
target_link_options(server_d10 PRIVATE -fsanitize=address)

# target_compile_options(server_d10 PRIVATE -fsanitize=thread -fno-omit-frame-pointer -O1)
# target_link_options(server_d10 PRIVATE -fsanitize=thread)

# target_compile_options(server_d10
#     PRIVATE
#     $<$<CONFIG:Release>:-O3>
#     $<$<CONFIG:RelWithDebInfo>:-O2>
# )

add_executable(client_d10
    client.cc
)

add_executable(ThreadPoolTest
    ThreadPoolTest.cc
)

add_executable(test
    test.cc
)

target_compile_options(test
    PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>:-O2>
)