cmake_minimum_required(VERSION 3.10)

project(servercpp LANGUAGES C CXX)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置静态库 (.a / .lib) 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置动态库 (.so / .dylib / .dll) 输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)

# 在顶层 CMakeLists.txt 中
option(WITH_EXAMPLES "Build examples" OFF)

# cmake -DWITH_EXAMPLES=ON .. 开启examples构建
if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()