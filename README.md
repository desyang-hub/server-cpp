# TCPServer-cpp

一个基于 **C++17** 的高性能 TCP 服务器框架，采用 **Multi-Reactor** 架构，基于 **epoll (ET 模式)** 实现非阻塞 I/O 多路复用。

## 特性

- **Multi-Reactor 架构** — Main Reactor 负责 accept 新连接，Sub Reactors 负责 I/O 读写，职责分离
- **epoll 边缘触发 (ET)** — 高效事件通知，配合非阻塞 I/O 减少系统调用
- **线程池** — Sub Reactors 由线程池驱动，默认线程数为 `hardware_concurrency`
- **Buffer 管理** — 三区段缓冲区（prependable | readable | writeable），支持自动扩容
- **智能指针生命周期管理** — `Connection` 使用 `shared_ptr` + `weak_ptr` 避免悬垂回调
- **静态/动态库构建** — 通过 `BUILD_SHARED_LIBS` 控制，支持 `find_package` 集成
- **AddressSanitizer** — Debug 模式自动启用 ASan，检测内存错误
- **CPack 打包** — 支持 TGZ/ZIP 格式发布

## 架构概览

```
┌─────────────────────────────────────────────────────┐
│                    TcpServer                         │
│                                                     │
│  ┌──────────────┐     ┌───────────────────────────┐ │
│  │ Main Reactor │     │      Sub Reactors         │ │
│  │  (EventLoop) │     │  ┌─────────┐ ┌─────────┐  │ │
│  │              │     │  │Reactor 0│ │Reactor 1│  │ │
│  │  ┌────────┐  │     │  │         │ │         │  │ │
│  │  │Acceptor│──┼──┐  │  │ (fd%N)  │ │ (fd%N)  │  │ │
│  │  └────────┘  │  │  │  └─────────┘ └─────────┘  │ │
│  └──────────────┘  │  │         ...                │ │
│                    │  └───────────────────────────┘ │
│                    │                                │
│              ┌─────▼──────┐                         │
│              │  Connection │  (fd → shared_ptr)     │
│              │  ┌─Socket─┐ │                        │
│              │  │─Channel│ │                        │
│              │  │─Buffer │ │                        │
│              │  └────────┘ │                        │
│              └─────────────┘                        │
└─────────────────────────────────────────────────────┘
```

**工作流程：**
1. `Main Reactor` 运行 `Acceptor`，监听新连接到来
2. 新连接通过 `fd % N` 算法分配到某个 `Sub Reactor`
3. `Sub Reactor` 的 epoll 以 ET 模式监听该连接的可读事件
4. 事件触发后，通过线程池执行回调，读取数据到 `Buffer`
5. 用户通过 `OnMessageCallBack` 处理业务逻辑

## 目录结构

```
server-cpp/
├── src/
│   ├── include/
│   │   ├── core/           # 核心组件头文件
│   │   │   ├── TcpServer.h
│   │   │   ├── EventLoop.h
│   │   │   ├── Epoll.h
│   │   │   ├── Channel.h
│   │   │   ├── Acceptor.h
│   │   │   └── Connection.h
│   │   ├── net/            # 网络层
│   │   │   ├── Socket.h
│   │   │   └── InetAddress.h
│   │   ├── base/           # 基础设施
│   │   │   ├── Buffer.h
│   │   │   └── ThreadPool.h
│   │   └── common/         # 公共工具
│   │       ├── utils.h
│   │       ├── ServerException.h
│   │       └── nonecopyable.h
│   ├── core/               # 核心组件实现
│   ├── net/                # 网络层实现
│   ├── base/               # 基础设施实现
│   ├── common/             # 公共工具实现
│   ├── apps/               # 示例服务端程序
│   └── cmake/              # CMake 配置模板
├── benchmarks/
│   ├── echo_server/        # Echo 服务器压测
│   └── tcpbench/           # TCP 压力测试工具
├── examples/               # 开发过程中的学习示例 (day01 ~ day12)
├── scripts/
│   └── build.sh            # 快捷构建脚本
├── docs/                   # 文档 & GitHub Pages
└── .github/workflows/      # CI/CD
```

## 快速开始

### 环境要求

- Linux（依赖 `epoll`）
- CMake >= 3.10
- 支持 C++17 的编译器（GCC 7+ / Clang 5+）

### 构建

```bash
# 克隆项目
git clone https://github.com/desyang/server-cpp.git
cd server-cpp

# 快捷构建（静态库 + Debug）
bash scripts/build.sh

# 或手动构建
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### 构建选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `CMAKE_BUILD_TYPE` | Debug | `Debug` / `Release` |
| `BUILD_SHARED_LIBS` | OFF | `ON` 构建动态库，`OFF` 构建静态库 |
| `WITH_EXAMPLES` | OFF | `ON` 构建 examples 目录中的示例 |

```bash
# 动态库 + Release + 构建示例
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DWITH_EXAMPLES=ON
cmake --build build -j$(nproc)
```

### 安装

```bash
cmake --install build --prefix ./tcpserver-install
```

安装后目录结构：
```
tcpserver-install/
├── include/          # 头文件
├── lib/              # 库文件 + CMake 配置
│   ├── libtcp_core.*
│   ├── libtcp_net.*
│   ├── libtcp_base.*
│   ├── libtcp_common.*
│   └── cmake/TCPServer/
└── bin/              # 可执行文件（可选）
```

### 在你的项目中使用

```cmake
# 设置安装路径（如果未安装到系统路径）
list(APPEND CMAKE_PREFIX_PATH "/path/to/tcpserver-install")

find_package(TCPServer REQUIRED)
target_link_libraries(your_target TCPServer::tcp_core)
```

## 使用示例

一个最简单的 Echo Server：

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
        int n = conn->send(buf->readerPos(), buf->readableBytes());
        buf->retrieve(n);
    });

    loop.loop();
    return 0;
}
```

## API 参考

### TcpServer

| 方法 | 说明 |
|------|------|
| `TcpServer(EventLoop* loop, const InetAddress& addr)` | 构造函数，绑定事件循环和监听地址 |
| `setOnConnection(OnConnectionCallBack)` | 设置连接建立回调 |
| `setOnMessageCallBack(OnMessageCallBack)` | 设置消息到达回调 |
| `setNewConnectionCallBack(NewConnectionCallBack)` | 设置新连接回调（内部使用） |
| `setDisConnectionCallBack(DisConnectionCallBack)` | 设置断开连接回调 |

### Connection

| 方法 | 说明 |
|------|------|
| `fd() const` | 获取文件描述符 |
| `send(const std::string&)` | 发送字符串数据 |
| `send(const char*, size_t)` | 发送原始数据 |
| `close()` | 关闭连接 |

### Buffer

| 方法 | 说明 |
|------|------|
| `readableBytes() const` | 可读字节数 |
| `writerableBytes() const` | 可写字节数 |
| `readerPos() const` | 可读数据起始指针 |
| `retrieve(size_t len)` | 消费 len 字节数据 |
| `readFd(int fd)` | 从 fd 读取数据到缓冲区 |
| `append(const char*, size_t)` | 追加数据到缓冲区 |

## 压力测试

项目提供了 `tcpbench` 压力测试工具：

```bash
# 构建 tcpbench
cd benchmarks/tcpbench && make

# 运行压测：100 并发，每连接 10000 消息
./tcpbench -t 100 -m 10000 -w 1
```

示例输出：
```
========== Stress Test Start ==========
Threads: 100 | Msgs/Thread: 10000 | Total Expected: 1000000

========== Stress Test Report ==========
Total Time      : 3.238 s
Success Requests: 1000000 / 1000000 (100.00%)
Failed Requests : 0
QPS (Throughput): 308832.61 req/s
Avg Latency     : 0.207 ms
=========================================
```

## CI/CD

项目配置了 GitHub Actions：

- **Build** — 推送到 `main`/`develop` 或 PR 时触发，矩阵测试 Debug/Release × Static/Dynamic
- **Release** — 推送 `v*.*.*` 标签时自动构建并发布 TGZ 包到 GitHub Releases

## 开发笔记

项目 `examples/` 目录记录了从零开始的学习过程（day01 ~ day12），涵盖 epoll 基础、Channel 封装、Reactor 模型、智能指针生命周期管理等主题。

## License

MIT
