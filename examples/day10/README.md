


### 默认情况下，socket中，向已经关闭的通道继续发送信息，会触发sigpipe信号， 默认直接终止程序


### day10 遇到的问题

- 发生了一些情况导致Connection类使用过程中出现被关闭的情况

解决方案，使用shared_ptr管理生命周期
& 继承 public std::enable_shared_from_this<Connection>

注意！！！ 不能再构造函数中使用shared_from_this() 方法

绑定 Channel ReadCallBack 时候，必须确保该回调被调用时，这个Connection类实例必须存在，所以使用shared_ptr来绑定一个实例，那么只要改Channel没有被销毁，对应的Connectioin就永远存在，所以解决了这个问题

这个异常非常难调试，必须谨慎处理


### 新问题点

int Socket::send(const std::string& msg) {
    return ::send(fd_, msg.c_str(), msg.size(), 0);
}

# 启用 AddressSanitizer
target_compile_options(server_d10 PRIVATE -fsanitize=address -fno-omit-frame-pointer -O1)
target_link_options(server_d10 PRIVATE -fsanitize=address)

启用后，通过脚本重复测试，获取异常点，此处