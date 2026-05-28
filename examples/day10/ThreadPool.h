#pragma once

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include "logger.h"

const int DEFAULT_POOL_SIZE = 8;

class ThreadPool
{
    using WorkerHandler = std::function<void()>;
private:
    std::queue<WorkerHandler> queue_;
    std::vector<std::thread> workers_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;

    bool isStop_;
public:
    explicit ThreadPool(size_t pool_size = DEFAULT_POOL_SIZE);
    ~ThreadPool();

    ThreadPool(const ThreadPool& pool) = delete;
    ThreadPool& operator=(const ThreadPool& pool) = delete;

public:
    template<class F, typename ...Args>
    std::future<typename std::result_of<F(Args...)>::type>
    enqueue(F&& f, Args&& ...args);
};

inline ThreadPool::ThreadPool(size_t pool_size) : isStop_(false) {
    workers_.reserve(pool_size);

    for (int i = 0; i < pool_size; ++i) {
        workers_.emplace_back([this]{
            WorkerHandler task{};

            while (true) {
                {
                    std::unique_lock<std::mutex> lock(mutex_);

                    condition_.wait(lock, [this]{
                        return isStop_ || !queue_.empty();
                    });

                    if (!queue_.empty()) {
                        task = std::move(queue_.front());
                        queue_.pop();
                    } else {
                        break;
                    }
                }

                // 需要自行处理异常
                task();
            }
        });
    }
}

inline ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        isStop_ = true;
    }
    condition_.notify_all();

    for (std::thread& th : workers_) {
        if (th.joinable()) {
            th.join();
        }
    }
}


template<class F, typename ...Args>
inline std::future<typename std::result_of<F(Args...)>::type>
ThreadPool::enqueue(F&& f, Args&& ...args) {
    using return_type = typename std::result_of<F(Args...)>::type;

    // 构造一个异步任务
    auto taskPtr = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> fut = taskPtr->get_future();

    // 将任务添加到任务队列
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // 假如线程池结束了
        if (isStop_) {
            throw std::runtime_error("enqueu in thread pool stoped.");
        }

        // 添加任务到队列
        queue_.push([taskPtr]{
            (*taskPtr)();
        });
    }
    // 唤醒线程来消费
    condition_.notify_one();

    return fut;
}