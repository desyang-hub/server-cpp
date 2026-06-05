#include "ThreadPool.h"

ThreadPool::ThreadPool(int pool_size) : is_running(true) {
    workers_.reserve(pool_size);

    for (int i = 0; i < pool_size; ++i) {
        workers_.emplace_back([this]{

            JobHandler job;

            while (true) {
                {
                    std::unique_lock<std::mutex> lock(que_mutex_);

                    condition_.wait(lock, [this]{
                        return !is_running || !que_.empty();
                    });

                    if (!que_.empty()) {
                        job = std::move(que_.front());
                        que_.pop();
                    }
                    else {
                        break;
                    }
                }
                
                job(); // 执行，任务需要自行处理异常
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(que_mutex_);
        is_running = false;
    }

    condition_.notify_all();

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}