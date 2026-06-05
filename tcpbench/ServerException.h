/**
 * @FilePath     : /server-cpp/test/ServerException.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 14:44:12
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 14:44:13
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
    ServerException(const std::string& msg) : msg_(msg) {}
    ~ServerException() = default;

    const char* what() const noexcept override {
        return msg_.c_str();
    }
};