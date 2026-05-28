#include <iostream>
#include <unistd.h>
#include <cstring>
#include <functional>
#include <atomic>
#include <chrono>
#include "utils.h"
#include "InetAddress.h"
#include "socket.h"
#include "ThreadPool.h"
#include "logger.h"

using namespace std;
using namespace chrono;

// ================= 全局无锁统计指标 =================
atomic<int> g_success_count{0};
atomic<int> g_fail_count{0};
atomic<long long> g_total_latency_us{0}; // 累计请求延迟(微秒)

void oneClient(int msgs, int wait) {
    Socket sock = Socket();
    sock.setTimeout(1);
    InetAddress addr = InetAddress(8080, "127.0.0.1");
    
    // 连接失败直接计入错误并返回，避免 errif 中断整个压测进程
    if (sock.connect(addr) == false) {
        g_fail_count.fetch_add(msgs, memory_order_relaxed);
        return;
    }

    if (wait > 0) sleep(wait);

    char buf[1024];
    const string payload = "I'm client!";

    for (int count = 0; count < msgs; ++count) {
        auto start = high_resolution_clock::now();

        // 发送数据
        int send_bytes = sock.send(payload.c_str());
        if (send_bytes == -1) {
            g_fail_count.fetch_add(1, memory_order_relaxed);
            continue;
        }

        // 接收响应
        int recv_bytes = sock.recv(buf, sizeof(buf));
        if (recv_bytes <= 0) {
            g_fail_count.fetch_add(1, memory_order_relaxed);
            continue;
        }

        auto end = high_resolution_clock::now();
        long long latency = duration_cast<microseconds>(end - start).count();

        // 纯原子操作更新统计，绝不加锁，避免统计本身成为压测瓶颈
        g_success_count.fetch_add(1, memory_order_relaxed);
        g_total_latency_us.fetch_add(latency, memory_order_relaxed);
    }
}

int main(int argc, char *argv[]) {
    int threads = 100;
    int msgs = 100;
    int wait = 0;

    // ================= 参数解析 =================
    int o;
    const char *optstring = "t:m:w:";
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't': threads = stoi(optarg); break;
            case 'm': msgs = stoi(optarg); break;
            case 'w': wait = stoi(optarg); break;
            default:
                printf("Usage: %s [-t threads] [-m msgs_per_thread] [-w wait_seconds]\n", argv[0]);
                return 1;
        }
    }

    int total_expected = threads * msgs;
    cout << "========== Stress Test Start ==========" << endl;
    cout << "Threads: " << threads 
         << " | Msgs/Thread: " << msgs 
         << " | Total Expected: " << total_expected << endl;

    // ================= 启动计时与任务 =================
    auto global_start = high_resolution_clock::now();

    {
        // ThreadPool 放在块作用域内
        // 析构时会自动 join 所有工作线程，确保任务全部执行完毕
        ThreadPool pool(threads);
        std::function<void()> func = std::bind(oneClient, msgs, wait);

        for (int i = 0; i < threads; ++i) {
            pool.enqueue(func);
        }
    } // <-- 离开作用域，ThreadPool 析构，阻塞直到所有客户端跑完

    auto global_end = high_resolution_clock::now();
    double total_time_s = duration_cast<milliseconds>(global_end - global_start).count() / 1000.0;

    // ================= 输出测试报告 =================
    int success = g_success_count.load(memory_order_relaxed);
    int fail = g_fail_count.load(memory_order_relaxed);
    long long total_lat = g_total_latency_us.load(memory_order_relaxed);

    double qps = (total_time_s > 0) ? (success / total_time_s) : 0;
    double avg_latency_ms = (success > 0) ? (total_lat / (double)success / 1000.0) : 0;
    double success_rate = (total_expected > 0) ? (success * 100.0 / total_expected) : 0;

    cout << "\n========== Stress Test Report ==========" << endl;
    printf("Total Time      : %.3f s\n", total_time_s);
    printf("Success Requests: %d / %d (%.2f%%)\n", success, total_expected, success_rate);
    printf("Failed Requests : %d\n", fail);
    printf("QPS (Throughput): %.2f req/s\n", qps);
    printf("Avg Latency     : %.3f ms\n", avg_latency_ms);
    cout << "=========================================" << endl;

    return (fail > 0) ? 1 : 0;
}