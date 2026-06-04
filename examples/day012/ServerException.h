#pragma once

#include <stdexcept>
#include <string>

class ServerException : std::exception
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