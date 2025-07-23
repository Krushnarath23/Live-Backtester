#include "core/strategy/strategy_factory.hpp"
#include "core/execution/portfolio.hpp"
#include "core/market/tick_data.hpp"
#include <cassert>
#include <iostream>

int main() {
    auto strategy = create_strategy("sma");
    Portfolio portfolio;

    std::vector<TickData> ticks;
    for (int i = 0; i < 30; ++i) {
        ticks.push_back(TickData{std::chrono::system_clock::now(), "BTCUSDT", 100.0 + i, 1.0});
    }

    for (const auto& tick : ticks) {
        strategy->on_tick(tick);
        auto orders = strategy->generate_orders();
        for (const auto& order : orders) {
            double qty = (order.side == OrderSide::BUY) ? 1.0 : -1.0;
            portfolio.update_position(order.symbol, qty);
            strategy->on_order_fill(order);
        }
    }

    double pos = portfolio.get_position("BTCUSDT");
    std::cout << "Final position: " << pos << std::endl;
    assert(pos >= -1.0 && pos <= 1.0); // only one BUY or SELL allowed at most
    return 0;
}
