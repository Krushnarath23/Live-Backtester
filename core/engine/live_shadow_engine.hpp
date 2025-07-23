#pragma once
#include "core/strategy/istrategy.hpp"
#include "core/execution/portfolio.hpp"
#include "core/execution/order.hpp"
#include "core/execution/slippage_model.hpp"
#include "core/utils/logging.hpp"
#include "core/utils/latency_simulator.hpp"
#include "core/market/tick_data.hpp"

#include <memory>
#include <vector>

class LiveShadowEngine {
public:
    LiveShadowEngine(std::unique_ptr<IStrategy> strat, Portfolio& port)
        : strategy(std::move(strat)), portfolio(port), slippage_model(0.03), latency_sim() {}

    void on_market_data(const TickData& tick) {
        strategy->on_tick(tick);
        auto orders = strategy->generate_orders();
        for (auto order : orders) {
            Logger::log(Logger::Level::INFO, "[LIVE] Virtual order: " + order.symbol);

            order.price = slippage_model.apply(order);
            order.timestamp = latency_sim.apply_latency(order.timestamp);

            virtual_execute(order);
            strategy->on_order_fill(order);
        }
    }

private:
    void virtual_execute(const Order& order) {
        double qty = (order.side == OrderSide::BUY) ? order.quantity : -order.quantity;
        portfolio.update_position(order.symbol, qty);
    }

    std::unique_ptr<IStrategy> strategy;
    Portfolio& portfolio;
    SlippageModel slippage_model;
    LatencySimulator latency_sim;
};
