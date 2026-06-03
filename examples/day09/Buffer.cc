#include "Buffer.h"

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
    readerIndex_ = writerIndex_;
}

std::string Buffer::retrieveAllAsString() {
    std::string bytes = retrieveAsString(readableBytes());
    readerIndex_ = 0;
    writerIndex_ = 0;
    return bytes;
}

// 将数据
void Buffer::append(const std::string& msg) {
    if (msg.empty()) return;

    // 1. 剩余空间足够写入
    if (writableBytes() >= msg.size()) {
        copyToBack(msg);
    } else if(writableBytes() + prependableBytes() <= msg.size()) { // 2. 空间足够，需要前移prepareable
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
    } else if(writableBytes() + prependableBytes() <= len) { // 2. 空间足够，需要前移prepareable
        copyToFront();
        copyToBack(buf, len);
    } else { // 3. 空间不足，前移后，先扩容继续将后半部分数据写入
        copyToFront();
        // 扩容
        store_.resize(readableBytes() + len);
        copyToBack(buf, len);
    }
}

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}

size_t Buffer::writableBytes() const {
    return store_.size() - readableBytes();
}

size_t Buffer::prependableBytes() const {
    return readerIndex_;
}