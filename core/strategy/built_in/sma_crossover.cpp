#include "core/strategy/istrategy.hpp"
#include "core/execution/order.hpp"
#include <deque>
#include <numeric>

class SmaCrossover : public IStrategy {
public:
    SmaCrossover(size_t short_window, size_t long_window, const std::string& symbol)
        : short_window(short_window), long_window(long_window), symbol(symbol) {}

    void on_tick(const TickData& tick) override {
        if (tick.symbol != symbol) return;

        prices.push_back(tick.price);
        if (prices.size() > long_window) prices.pop_front();

        if (prices.size() >= long_window) {
            double short_sma = average(prices.end() - short_window, prices.end());
            double long_sma = average(prices.begin(), prices.end());

            if (!position_open && short_sma > long_sma) {
                pending_orders.emplace_back(Order{symbol, OrderType::MARKET, OrderSide::BUY, tick.price, 1.0, tick.timestamp});
                position_open = true;
            } else if (position_open && short_sma < long_sma) {
                pending_orders.emplace_back(Order{symbol, OrderType::MARKET, OrderSide::SELL, tick.price, 1.0, tick.timestamp});
                position_open = false;
            }
        }
    }

    std::vector<Order> generate_orders() override {
        auto orders = pending_orders;
        pending_orders.clear();
        return orders;
    }

    void on_order_fill(const Order&) override {}
    void reset() override {
        prices.clear();
        pending_orders.clear();
        position_open = false;
    }

private:
    template<typename Iter>
    double average(Iter begin, Iter end) {
        return std::accumulate(begin, end, 0.0) / std::distance(begin, end);
    }

    size_t short_window;
    size_t long_window;
    std::string symbol;
    std::deque<double> prices;
    std::vector<Order> pending_orders;
    bool position_open = false;
};
