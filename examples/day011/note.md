* 添加Channel类型
* 改造Epoll类型
* Channel构造函数初始化
* 添加Acceptor用于单独处理用户的连接逻辑

* 添加并实现Connection类型，用于管理Tcp连接

* 添加Buffer缓冲区


### 遇到问题，对于Connection的管理，由于很多过程使用this指针进行lambda函数捕获，所以移动和拷贝是棘手的问题，尽量避免拷贝和移动，而是使用智能指针进行管理


### 当前添加了线程池来执行，所以需要考虑并发问题
* 发生死锁了，似乎线程池中的任务一直没有结束 ?? 忘记设置connection回调函数了

Connection 和 Channel都用shared_ptr来管理

出问题了，似乎所有的fd都没有被关闭，需要调试Connection的生命周期，其对Socket进行管理

内存泄露了，所有的Connection建立后似乎都没有析构

#### 找到问题了，使用shared_ptr过程中，发生了循环引用的问题，导致成员变量持有自己的shared_ptr引用，所以资源永远无法释放，这个场景就可以用到weak_ptr智能指针，其并不持有shared_ptr资源，而是在需要时，尝试获取


### day011 需要添加主从多Reactor模式

wsl 下测试结果，性能优异 cpu core x 6

(base) root@DESKTOP-U42DJ8K:/home/desyang/github/server-cpp/build/examples/day11# ./test_d11 -t 1000 -m 100
========== Stress Test Start ==========
Threads: 1000 | Msgs/Thread: 100 | Total Expected: 100000

========== Stress Test Report ==========
Total Time      : 0.347 s
Success Requests: 100000 / 100000 (100.00%)
Failed Requests : 0
QPS (Throughput): 288184.44 req/s
Avg Latency     : 1.630 ms
=========================================
