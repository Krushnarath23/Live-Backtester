#pragma once
#include <functional>
#include <queue>
#include <chrono>
#include <map>

class EventLoop {
public:
    using EventCallback = std::function<void()>;
    using TimePoint = std::chrono::system_clock::time_point;

    void schedule(const TimePoint& time, EventCallback cb) {
        events.emplace(time, std::move(cb));
    }

    void run() {
        while (!events.empty()) {
            auto [time, cb] = *events.begin();
            if (std::chrono::system_clock::now() < time) break;
            cb();
            events.erase(events.begin());
        }
    }

private:
    std::multimap<TimePoint, EventCallback> events;
};
