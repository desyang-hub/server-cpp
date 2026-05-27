#include "ThreadPool.h"
#include <thread>
#include <chrono>
#include <iostream>

int add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a + b;
}

int main(int argc, char const *argv[])
{
    ThreadPool pool;

    int num = 4;
    std::vector<std::future<int>> results;
    results.reserve(num);

    for (int i = 0; i < num; ++i) {
        results.push_back(pool.enqueue(add, i, i + 1));
    }

    for (int i = 0; i < num; ++i) {
        std::cout << i << " " << results[i].get() << std::endl;
    }
    return 0;
}
