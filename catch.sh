#!/bin/bash
# asan_loop.sh: 专门用于捕获常驻服务偶现内存错误的脚本



MAX_RUNS=100
TIMEOUT_SEC=3  # 根据你之前压测耗时调整，确保能跑完一轮完整测试

for i in $(seq 1 $MAX_RUNS); do
    echo "=== Run $i / $MAX_RUNS ==="
    
    # 1. 清理可能残留的旧日志和进程
    rm -f asan.log
    pkill -9 server_d10 2>/dev/null
    
    # 2. 启动服务端（后台运行），ASan 默认输出到 stderr
    # ASAN_OPTIONS: 
    #   detect_leaks=0 关闭内存泄漏检测(服务端常驻必有泄漏，避免干扰)
    #   log_path=asan.log 将ASan报告重定向到文件，避免刷屏
    ASAN_OPTIONS="detect_leaks=0:log_path=asan.log" \
    ./server_d10 > /dev/null 2>&1 &
    SERVER_PID=$!

    sleep 2
    # 启动服务端后，立即发起压测
    ./test -t 100 -m 1000 &
    CLIENT_PID=$!

    # 等待压测完成或超时
    wait $CLIENT_PID 2>/dev/null

    # 然后再检查服务端是否存活...
    
    # 3. 等待指定时间，让压测客户端有足够时间触发bug
    sleep $TIMEOUT_SEC
    
    # 4. 检查进程是否还活着
    if ! kill -0 $SERVER_PID 2>/dev/null; then
        echo "💥 CRASH DETECTED at run $i!"
        wait $SERVER_PID 2>/dev/null  # 回收僵尸进程
        
        # 打印 ASan 报告
        if [ -s asan.log.* ]; then
            echo "===== ASan Report ====="
            cat asan.log.*
        fi
        exit 1  # 抓到 bug，停止循环
    fi
    
    # 5. 本轮未崩溃，正常杀掉进入下一轮
    kill -TERM $SERVER_PID 2>/dev/null
    wait $SERVER_PID 2>/dev/null
    echo "✅ Run $i passed"
done

echo "🎉 All $MAX_RUNS runs completed without crash."