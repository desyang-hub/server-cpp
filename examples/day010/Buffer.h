#pragma once

#include <vector>
#include <stddef.h>
#include <iostream>
#include <assert.h>

// |prepareable|readable|writeable| 缓冲区样式
// 当空间充足时，在readable处写入数据

const size_t DEFAULT_STORE_SIZE = 1024;

class Buffer {
public:
    std::vector<char> store_;

private:
    size_t readerIndex_;   // 写入的数据起始点
    size_t writerIndex_;  // 写入数据的结束点

private:
    void copyToBack(const std::string& msg);

    void copyToBack(const char* buf, size_t len);

    void copyToFront();
    
public:
    Buffer(size_t default_store_size=DEFAULT_STORE_SIZE);

public:
    // 将prepareable_和readable光标向前移动
    void retrieve(size_t len);

    // 移动并将长度为len的字符串返回
    std::string retrieveAsString(size_t len);

    void retrieveAll();

    std::string retrieveAllAsString();

    // 将数据
    void append(const std::string& msg);

    void append(const char* buf, size_t len);

    size_t readableBytes() const;

    size_t writableBytes() const;

    size_t prependableBytes() const;
};