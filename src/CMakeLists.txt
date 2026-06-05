set(TCPSERVER_COMMON tcp_common)
set(TCPSERVER_BASE tcp_base)
set(TCPSERVER_NET tcp_net)
set(TCPSERVER_CORE tcp_core)
set(APPS_SERVER apps_server)

# =====================

include_directories(${TCPSERVER} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# =====================
# 辅助函数：启用 ASan（仅 Debug）
# =====================
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

# =====================
# 库定义（自动根据 BUILD_SHARED_LIBS 决定 STATIC/SHARED）
# =====================

# 使用 OBJECT 库或直接指定类型？→ 更简单：用变量控制
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

add_library(${TCPSERVER_COMMON} ${LIB_TYPE}
    common/ServerException.cc
    common/utils.cc
)

add_library(${TCPSERVER_BASE} ${LIB_TYPE}
    base/Buffer.cc
    base/ThreadPool.cc
)

add_library(${TCPSERVER_NET} ${LIB_TYPE}
    net/InetAddress.cc
    net/Socket.cc
)

target_link_libraries(${TCPSERVER_NET}
    ${TCPSERVER_BASE}
    ${TCPSERVER_COMMON}
)

add_library(${TCPSERVER_CORE} ${LIB_TYPE}
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


# 假设你的 include 目录是 ${PROJECT_SOURCE_DIR}/include
set(INCLUDE_INSTALL_DIR "include")

# 设置每个库的 PUBLIC 头文件路径（关键！）
target_include_directories(tcp_base PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_include_directories(tcp_common PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_include_directories(tcp_net PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

target_include_directories(tcp_core PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

# =====================
# 可执行文件
# =====================
add_executable(${APPS_SERVER}
    apps/server.cc
)

target_link_libraries(${APPS_SERVER}
    ${TCPSERVER_CORE}
    ${TCPSERVER_NET}
    ${TCPSERVER_BASE}
    ${TCPSERVER_COMMON}
)

# =====================
# 编译优化 & 调试选项
# =====================

# Release 优化
target_compile_options(${APPS_SERVER} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_COMMON} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_BASE} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_NET} PRIVATE $<$<CONFIG:Release>:-O3>)
target_compile_options(${TCPSERVER_CORE} PRIVATE $<$<CONFIG:Release>:-O3>)

# Debug ASan（所有目标）
enable_asan(${TCPSERVER_NET})
enable_asan(${TCPSERVER_CORE})
enable_asan(${TCPSERVER_BASE})
enable_asan(${TCPSERVER_COMMON})
enable_asan(${APPS_SERVER})

# =====================
# 安装规则（用于 make install 和 CPack）
# =====================

# 头文件安装路径
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)

# 库文件安装
install(TARGETS ${TCPSERVER_COMMON} ${TCPSERVER_BASE} ${TCPSERVER_NET} ${TCPSERVER_CORE}
    EXPORT TCPServerTargets
    LIBRARY DESTINATION lib      # .so 文件
    ARCHIVE DESTINATION lib      # .a 文件
    RUNTIME DESTINATION bin      # Windows .dll（Linux 不适用）
)

# 可执行文件安装（可选）
if(INSTALL_APPS_SERVER)
    install(TARGETS ${APPS_SERVER}
        DESTINATION bin
    )
endif()

# =====================
# 导出配置（供其他项目 find_package 使用）
# =====================
include(CMakePackageConfigHelpers)

# 生成 TCPServerConfig.cmake
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfigVersion.cmake"
    VERSION 1.0
    COMPATIBILITY AnyNewerVersion
)

# 安装配置文件
install(EXPORT TCPServerTargets
    FILE TCPServerTargets.cmake
    NAMESPACE TCPServer::
    DESTINATION lib/cmake/TCPServer
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfigVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TCPServerConfig.cmake.in"  # ← 你需要创建这个文件（见下方说明）
    DESTINATION lib/cmake/TCPServer
)



# --- 安装 CMake 配置文件 ---
include(CMakePackageConfigHelpers)

# 生成 TCPServerConfigVersion.cmake
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

# 生成 TCPServerConfig.cmake（从 .in 模板）
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TCPServerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/TCPServer"
)

# 安装这些生成的文件
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TCPServerConfigVersion.cmake"
    DESTINATION "lib/cmake/TCPServer"
)

# =====================
# CPack 打包配置
# =====================
set(CPACK_PACKAGE_NAME "tcpserver")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High-performance TCP server library")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# 生成 tar.gz / zip
set(CPACK_GENERATOR "TGZ;ZIP")

include(CPack)