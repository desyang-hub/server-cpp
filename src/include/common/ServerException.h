/**
 * @FilePath     : /server-cpp/src/include/common/ServerException.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:05
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <stdexcept>
#include <string>

class ServerException : public std::exception
{
private:
    std::string msg_;
public:
    ServerException(const std::string& msg);
    ~ServerException() = default;

    const char* what() const noexcept override;
};