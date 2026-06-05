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

# 发布模式设置优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # 或多配置生成器（如 Visual Studio）
    target_compile_options(${APPS_SERVER} PRIVATE $<$<CONFIG:Release>:-O3>)
    target_compile_options(${TCPSERVER_COMMON} PRIVATE $<$<CONFIG:Release>:-O3>)
    target_compile_options(${TCPSERVER_BASE} PRIVATE $<$<CONFIG:Release>:-O3>)
    target_compile_options(${TCPSERVER_NET} PRIVATE $<$<CONFIG:Release>:-O3>)
    target_compile_options(${TCPSERVER_CORE} PRIVATE $<$<CONFIG:Release>:-O3>)    
endif()