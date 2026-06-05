# 一个简单的TCP压力测试工具

```bash
# -t并发访问数量，-m持续发送的消息数量，-w连接后等待时长
./tcpbench -t 100  -m 10000 -w 1

# 测试结果
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
