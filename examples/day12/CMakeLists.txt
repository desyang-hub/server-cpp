set(LIB_NAME lib_d12)

add_subdirectory(src)

link_libraries(${LIB_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server_d12
    server.cc
)

# 启用 AddressSanitizer
# target_compile_options(server_d12 PRIVATE -fsanitize=address -fno-omit-frame-pointer -O1)
# target_link_options(server_d12 PRIVATE -fsanitize=address)

# target_compile_options(server_d12 PRIVATE -fsanitize=thread -fno-omit-frame-pointer -O1)
# target_link_options(server_d12 PRIVATE -fsanitize=thread)

target_compile_options(server_d12
    PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>:-O2>
)

add_executable(client_d12
    client.cc
)

add_executable(ThreadPoolTest_d12
    ThreadPoolTest.cc
)

add_executable(test_d12
    test.cc
)

target_compile_options(test_d12
    PRIVATE
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>:-O2>
)