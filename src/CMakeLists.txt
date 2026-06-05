set(TCPSERVER_COMMON tcp_common)
set(TCPSERVER_BASE tcp_base)
set(TCPSERVER_NET tcp_net)
set(TCPSERVER_CORE tcp_core)
set(APPS_SERVER apps_server)

# =====================

include_directories(${TCPSERVER} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(${TCPSERVER_COMMON}
    common/ServerException.cc
    common/utils.cc
)

add_library(${TCPSERVER_BASE}
    base/Buffer.cc
    base/ThreadPool.cc
)

add_library(${TCPSERVER_NET}
    net/InetAddress.cc
    net/Socket.cc
)

target_link_libraries(${TCPSERVER_NET}
    ${TCPSERVER_BASE}
    ${TCPSERVER_COMMON}
)

add_library(${TCPSERVER_CORE}
    core/Acceptor.cc
    core/Channel.cc
    core/Connection.cc
    core/Epoll.cc
    core/EventLoop.cc
    core/TcpServer.cc
)

target_link_libraries(${TCPSERVER_CORE}
    ${TCPSERVER_NET}
    ${TCPSERVER_BASE}
    ${TCPSERVER_COMMON}
)

add_executable(${APPS_SERVER}
    apps/server.cc
)

target_link_libraries(${APPS_SERVER}
    ${TCPSERVER_CORE}
    ${TCPSERVER_NET}
    ${TCPSERVER_BASE}
    ${TCPSERVER_COMMON}
)

# 定义一个函数，方便统一设置 sanitizer
function(enable_asan TARGET_NAME)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET_NAME} PRIVATE 
            $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer -O1>
        )
        target_link_options(${TARGET_NAME} PRIVATE 
            $<$<CONFIG:Debug>:-fsanitize=address>
        )
    endif()
endfunction()

# Release 优化（适用于所有生成器）
target_compile_options(${APPS_SERVER} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_COMMON} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_BASE} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_NET} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_CORE} PRIVATE $<$<CONFIG:Release>:-O3>)

# Debug 调试选项（适用于所有生成器）
# 为所有库和可执行文件启用 ASan
enable_asan(${TCPSERVER_NET})
enable_asan(${TCPSERVER_CORE})
enable_asan(${TCPSERVER_BASE})
enable_asan(${TCPSERVER_COMMON})
enable_asan(${APPS_SERVER})   # ←←← 关键！可执行文件也要加