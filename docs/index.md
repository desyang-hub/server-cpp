---
layout: default
title: TCPServer-cpp
description: 一个基于 C++17 的高性能 TCP 服务器框架，采用 Multi-Reactor 架构
---

# TCPServer-cpp

<div class="badges">
  <img src="https://img.shields.io/badge/language-C%2B%2B17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/architecture-Multi--Reactor-green.svg" alt="Multi-Reactor">
  <img src="https://img.shields.io/badge/platform-Linux-lightgrey.svg" alt="Linux">
  <img src="https://img.shields.io/badge/license-MIT-yellow.svg" alt="MIT">
</div>

一个基于 **C++17** 的高性能 TCP 服务器框架，采用 **Multi-Reactor** 架构，基于 **epoll (ET 模式)** 实现非阻塞 I/O 多路复用。

---

## 为什么选择 TCPServer-cpp？

- 🚀 **高性能** — Multi-Reactor + epoll ET + 非阻塞 I/O，单机 QPS 可达 30 万+
- 🧩 **模块化设计** — 核心层、网络层、基础层职责清晰，易于扩展
- 🔒 **安全的生命周期管理** — shared_ptr/weak_ptr 避免悬垂回调和内存泄漏
- 🛠️ **开箱即用** — CMake 构建 + CPack 打包 + GitHub Actions CI/CD

---

## 架构

<div class="architecture">
<pre>
┌──────────────────────────────────────────────────────┐
│                     TcpServer                         │
│                                                      │
│   ┌──────────────┐      ┌──────────────────────────┐ │
│   │ Main Reactor │      │      Sub Reactors        │ │
│   │  (EventLoop) │      │  ┌────────┐ ┌────────┐   │ │
│   │              │      │  │Reactor │ │Reactor │   │ │
│   │  ┌────────┐  │      │  │   0    │ │   1    │   │ │
│   │  │Acceptor│──┼──┐   │  └────────┘ └────────┘   │ │
│   │  └────────┘  │  │   │         ...               │ │
│   └──────────────┘  │   └──────────────────────────┘ │
│                     │                                │
│               ┌─────▼─────┐                          │
│               │Connection │  fd → shared_ptr          │
│               │ Socket    │                           │
│               │ Channel   │                           │
│               │ Buffer    │                           │
│               └───────────┘                           │
└──────────────────────────────────────────────────────┘
</pre>
</div>

### 工作流程

1. **Main Reactor** 运行 `Acceptor`，监听新连接
2. 新连接通过 `fd % N` 算法分配到某个 **Sub Reactor**
3. Sub Reactor 的 epoll 以 **ET 模式**监听连接的可读事件
4. 事件触发后，通过**线程池**执行回调，读取数据到 `Buffer`
5. 用户通过 `OnMessageCallBack` 处理业务逻辑

---

## 核心组件

<div class="components">

### EventLoop
事件循环，封装 epoll 的 poll → dispatch 循环。每个 Reactor 对应一个 EventLoop 实例。

### Channel
文件描述符的事件抽象，管理 fd 的事件注册、触发回调。支持 EPOLLIN / EPOLLET。

### Acceptor
监听 socket 的封装，在 Main Reactor 中 accept 新连接并分发到 Sub Reactors。

### Connection
连接的完整抽象，持有 Socket + Channel + Buffer。通过 `shared_ptr` 管理生命周期，`weak_ptr` 绑定回调避免循环引用。

### Buffer
三区段缓冲区（prependable | readable | writeable），支持从 fd 直接读取、自动扩容、零拷贝消费。

### ThreadPool
基于 `std::future` 的线程池，驱动 Sub Reactors 的事件处理。

</div>

---

## 快速开始

### 环境要求

| 依赖 | 版本 |
|------|------|
| OS | Linux（依赖 epoll） |
| CMake | >= 3.10 |
| 编译器 | GCC 7+ / Clang 5+（C++17） |

### 构建

```bash
git clone https://github.com/desyang/server-cpp.git
cd server-cpp

# 快捷构建
bash scripts/build.sh

# 或手动构建
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### 构建选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `CMAKE_BUILD_TYPE` | Debug | Debug / Release |
| `BUILD_SHARED_LIBS` | OFF | ON 动态库 / OFF 静态库 |
| `WITH_EXAMPLES` | OFF | ON 构建 examples |

---

## 代码示例

### Echo Server

```cpp
#include "core/TcpServer.h"
#include "core/EventLoop.h"
#include "core/Connection.h"
#include "base/Buffer.h"
#include "net/InetAddress.h"

int main() {
    EventLoop loop;
    InetAddress addr(8080);
    TcpServer server(&loop, addr);

    server.setOnConnection([](Connection* conn) {
        std::cout << "fd=" << conn->fd() << " connected" << std::endl;
    });

    server.setOnMessageCallBack([](Connection* conn, Buffer* buf) {
        // 原样回显
        int n = conn->send(buf->readerPos(), buf->readableBytes());
        buf->retrieve(n);
    });

    loop.loop();
    return 0;
}
```

### 在你的项目中集成

```cmake
list(APPEND CMAKE_PREFIX_PATH "/path/to/tcpserver-install")
find_package(TCPServer REQUIRED)
target_link_libraries(your_target TCPServer::tcp_core)
```

---

## 压力测试

项目内置 `tcpbench` 压力测试工具：

```bash
# 构建
cd benchmarks/tcpbench && make

# 100 并发，每连接 10000 消息
./tcpbench -t 100 -m 10000 -w 1
```

### 测试结果

```
========== Stress Test Report ==========
Total Time      : 3.238 s
Success Requests: 1000000 / 1000000 (100.00%)
Failed Requests : 0
QPS (Throughput): 308832.61 req/s
Avg Latency     : 0.207 ms
=========================================
```

---

## 目录结构

```
server-cpp/
├── src/
│   ├── include/          # 头文件（core / net / base / common）
│   ├── core/             # EventLoop, Epoll, Channel, Acceptor, Connection, TcpServer
│   ├── net/              # Socket, InetAddress
│   ├── base/             # Buffer, ThreadPool
│   ├── common/           # utils, ServerException, nonecopyable
│   ├── apps/             # 服务端示例程序
│   └── cmake/            # CMake 包配置模板
├── benchmarks/
│   ├── echo_server/      # Echo Server
│   └── tcpbench/         # 压力测试工具
├── examples/             # 学习过程代码 (day01 ~ day12)
├── scripts/              # 构建脚本
├── docs/                 # 文档
└── .github/workflows/    # CI/CD
```

---

## CI/CD

<div class="ci">

| 工作流 | 触发条件 | 说明 |
|--------|----------|------|
| **Build** | push main/develop, PR | 矩阵测试 Debug/Release × Static/Dynamic |
| **Release** | push `v*.*.*` tag | 自动构建并发布 TGZ 到 GitHub Releases |

</div>

---

## 许可证

MIT License © desyang
