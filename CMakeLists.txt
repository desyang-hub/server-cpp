cmake_minimum_required(VERSION 3.10)

project(servercpp LANGUAGES C CXX)

set(SOCKET_LIB socket)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(${SOCKET_LIB}
    src/socket.cc
)

add_subdirectory(examples)
add_subdirectory(test)