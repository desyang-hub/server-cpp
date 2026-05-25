add_executable(socket_test socket_test.cc)
target_link_libraries(socket_test
    ${SOCKET_LIB}
)