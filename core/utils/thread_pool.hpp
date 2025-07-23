#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    explicit ThreadPool(size_t n) {
        for (size_t i = 0; i < n; ++i)
            workers.emplace_back([this] { worker(); });
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mu);
            shutdown = true;
        }
        cv.notify_all();
        for (auto& t : workers) t.join();
    }

    void submit(std::function<void()> job) {
        {
            std::lock_guard<std::mutex> lock(mu);
            tasks.push(std::move(job));
        }
        cv.notify_one();
    }

private:
    void worker() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mu);
                cv.wait(lock, [&] { return shutdown || !tasks.empty(); });
                if (shutdown && tasks.empty()) return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mu;
    std::condition_variable cv;
    bool shutdown = false;
};
