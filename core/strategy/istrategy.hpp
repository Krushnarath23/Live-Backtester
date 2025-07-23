#pragma once
#include "core/market/tick_data.hpp"
#include "core/execution/order.hpp"

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual void on_tick(const TickData& tick) = 0;
    virtual std::vector<Order> generate_orders() = 0;
    virtual void on_order_fill(const Order& order) = 0;
    virtual void reset() = 0;
};
