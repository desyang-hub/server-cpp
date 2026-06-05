/**
 * @FilePath     : /server-cpp/test/Buffer.cc
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 14:48:56
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 14:49:11
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/

#include "Buffer.h"

#include <sys/socket.h>
#include <sys/uio.h>

void Buffer::copyToBack(const std::string& msg) {
    std::copy(msg.begin(), msg.end(), store_.data() + writerIndex_);
    writerIndex_ += msg.size();
}

void Buffer::copyToBack(const char* buf, size_t len) {
    std::copy(buf, buf + len, store_.data() + writerIndex_);
    writerIndex_ += len;
}

void Buffer::copyToFront() {
    std::copy(store_.data() + readerIndex_, store_.data() + writerIndex_, store_.begin());
    writerIndex_ = readableBytes();
    readerIndex_ = 0;
}


Buffer::Buffer(size_t default_store_size) : readerIndex_(0), writerIndex_(0) {
    store_.resize(default_store_size);
}


// 将prepareable_和readable光标向前移动
void Buffer::retrieve(size_t len) {
    assert(readerIndex_ + len <= writerIndex_);
    readerIndex_ += len;
}

// 移动并将长度为len的字符串返回
std::string Buffer::retrieveAsString(size_t len) {
    std::string bytes(store_.data() + readerIndex_, len);
    retrieve(len);
    return bytes;
}

void Buffer::retrieveAll() {
    readerIndex_ = 0;
    writerIndex_ = 0;
}

std::string Buffer::retrieveAllAsString() {
    std::string bytes = retrieveAsString(readableBytes());
    retrieveAll();
    return bytes;
}

// 将数据
void Buffer::append(const std::string& msg) {
    if (msg.empty()) return;

    // 1. 剩余空间足够写入
    if (writableBytes() >= msg.size()) {
        copyToBack(msg);
    } else if(writableBytes() + prependableBytes() >= msg.size()) { // 2. 空间足够，需要前移prepareable
        copyToFront();
        copyToBack(msg);
    } else { // 3. 空间不足，前移后，先扩容继续将后半部分数据写入
        copyToFront();
        // 扩容
        store_.resize(readableBytes() + msg.size());
        copyToBack(msg);
    }
}

void Buffer::append(const char* buf, size_t len) {
    if (len == 0) return;

    // 1. 剩余空间足够写入
    if (writableBytes() >= len) {
        copyToBack(buf, len);
    } else if(writableBytes() + prependableBytes() >= len) { // 2. 空间足够，需要前移prepareable
        copyToFront();
        copyToBack(buf, len);
    } else { // 3. 空间不足，前移后，先扩容继续将后半部分数据写入
        copyToFront();
        // 扩容
        store_.resize(readableBytes() + len);
        copyToBack(buf, len);
    }
}

char* Buffer::begin() {
    return store_.data();
}

// Buffer.cc
ssize_t Buffer::readFd(int fd) {
    // 1. 准备额外缓冲区（栈上分配，高效）
    char extraBuf[65536];

    // 2. 构造 iovec 数组
    struct iovec vec[2];
    const size_t writable = writableBytes(); // buffer_ 中剩余空间

    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof(extraBuf);

    // 3. 调用 readv：一次 syscall 读入两个缓冲区
    const int iovcnt = (writable < sizeof(extraBuf)) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);

    if (n < 0) {
        // 错误处理
        return n;
    }

    // 4. 根据读取的字节数更新 Buffer
    if (static_cast<size_t>(n) <= writable) {
        // 数据全部写入原有 buffer
        writerIndex_ += n;
    } else {
        // 原有 buffer 写满，多余的数据在 extraBuf 中
        writerIndex_ = store_.size(); // 标记原 buffer 已满
        append(extraBuf, n - writable); // 自动扩容并追加
    }

    return n;
}

char* Buffer::readerPos() {
    return store_.data() + readerIndex_;
}

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}

size_t Buffer::writableBytes() const {
    return store_.size() - writerIndex_;
}

size_t Buffer::prependableBytes() const {
    return readerIndex_;
}