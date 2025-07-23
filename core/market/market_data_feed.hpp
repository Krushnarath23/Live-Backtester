#pragma once
#include "tick_data.hpp"
#include <vector>
#include <functional>

class MarketDataFeed {
public:
    using Callback = std::function<void(const TickData&)>;

    void subscribe(Callback cb) {
        callbacks.push_back(cb);
    }

    void publish(const TickData& tick) {
        for (auto& cb : callbacks) {
            cb(tick);
        }
    }

private:
    std::vector<Callback> callbacks;
};
