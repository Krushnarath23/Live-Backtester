#pragma once
#include "core/market/tick_data.hpp"
#include "core/strategy/istrategy.hpp"
#include "core/execution/order.hpp"
#include "core/execution/portfolio.hpp"
#include "core/execution/slippage_model.hpp"
#include "core/utils/logging.hpp"
#include "core/utils/latency_simulator.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

class BacktestEngine {
public:
    BacktestEngine(std::unordered_map<std::string, std::unique_ptr<IStrategy>> strategies, Portfolio& port)
        : strategies(std::move(strategies)), portfolio(port), slippage_model(0.05), latency_sim() {}

    void run(const std::vector<TickData>& market_data) {
        for (const auto& tick : market_data) {
            auto it = strategies.find(tick.symbol);
            if (it != strategies.end()) {
                auto& strat = it->second;
                strat->on_tick(tick);
                auto orders = strat->generate_orders();
                for (auto order : orders) {
                    Logger::log(Logger::Level::INFO, "Executing order: " + order.symbol);

                    order.price = slippage_model.apply(order);
                    order.timestamp = latency_sim.apply_latency(order.timestamp);

                    portfolio.log_trade(order);
                    strat->on_order_fill(order);
                }
            }
        }

        double pnl = portfolio.calculate_realized_pnl();
        Logger::log(Logger::Level::INFO, "Realized P&L: " + std::to_string(pnl));
    }

private:
    std::unordered_map<std::string, std::unique_ptr<IStrategy>> strategies;
    Portfolio& portfolio;
    SlippageModel slippage_model;
    LatencySimulator latency_sim;
};
