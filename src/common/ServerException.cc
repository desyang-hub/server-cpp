/**
 * @FilePath     : /server-cpp/src/common/ServerException.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 16:29:42
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:44
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#include "common/ServerException.h"

ServerException::ServerException(const std::string& msg) : msg_(msg) {}

const char* ServerException::what() const noexcept {
    return msg_.c_str();
}