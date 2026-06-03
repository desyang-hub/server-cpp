* 添加Channel类型
* 改造Epoll类型
* Channel构造函数初始化
* 添加Acceptor用于单独处理用户的连接逻辑

* 添加并实现Connection类型，用于管理Tcp连接

* 添加Buffer缓冲区


### 遇到问题，对于Connection的管理，由于很多过程使用this指针进行lambda函数捕获，所以移动和拷贝是棘手的问题，尽量避免拷贝和移动，而是使用智能指针进行管理