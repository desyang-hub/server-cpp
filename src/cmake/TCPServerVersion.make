configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/tcpserver/version.h"
    @ONLY
)

# ✅ 为所有需要版本信息的目标添加 include 路径
foreach(_target ${TCPSERVER_COMMON} ${TCPSERVER_BASE} ${TCPSERVER_NET} ${TCPSERVER_CORE})
    if(TARGET ${_target})
        target_include_directories(${_target} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:include>
        )
    endif()
endforeach()