/**
 * @FilePath     : /server-cpp/src/include/base/ThreadPool.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:38:59
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <thread>
#include <future>
#include <utility>
#include <functional>

const int THREAD_POOL_DEFAULT_SIZE = 6;

class ThreadPool
{
    using JobHandler = std::function<void()>;
private:
    /* data */
    std::vector<std::thread> workers_;
    std::queue<JobHandler> que_;
    std::mutex que_mutex_;
    std::condition_variable condition_;
    bool is_running;

public:
    ThreadPool(int pool_size = THREAD_POOL_DEFAULT_SIZE);
    ~ThreadPool();

public:
    template<class F, typename ...Args>
    auto enqueue(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args...)>::type>;
};


template<class F, typename ...Args>
inline auto ThreadPool::enqueue(F&& f, Args&& ...args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    // 构造一个异步任务
    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();

    {
        std::lock_guard<std::mutex> lock(que_mutex_);

        if (!is_running) {
            throw std::runtime_error("enqueue in thread pool stoped");
        }

        que_.push([task]{
            (*task)();
        });
    }

    condition_.notify_one();

    return res;
}