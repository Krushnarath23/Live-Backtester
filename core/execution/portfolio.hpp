#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include "order.hpp"

struct Trade {
    std::string symbol;
    double price;
    double quantity;
    std::chrono::system_clock::time_point timestamp;
    OrderSide side;
};

class Portfolio {
public:
    void update_position(const std::string& symbol, double quantity) {
        positions[symbol] += quantity;
    }

    double get_position(const std::string& symbol) const {
        auto it = positions.find(symbol);
        return it != positions.end() ? it->second : 0.0;
    }

    void log_trade(const Order& order) {
        trades.push_back({order.symbol, order.price, order.quantity, order.timestamp, order.side});
        update_position(order.symbol, order.side == OrderSide::BUY ? order.quantity : -order.quantity);
    }

    const std::vector<Trade>& get_trades() const {
        return trades;
    }

    const std::unordered_map<std::string, double>& all_positions() const {
        return positions;
    }

    double calculate_realized_pnl() const {
        std::unordered_map<std::string, double> avg_price;
        std::unordered_map<std::string, double> quantity;
        double realized_pnl = 0.0;

        for (const auto& trade : trades) {
            double qty = trade.side == OrderSide::BUY ? trade.quantity : -trade.quantity;
            double current_pos = quantity[trade.symbol];

            if ((current_pos > 0 && qty < 0) || (current_pos < 0 && qty > 0)) {
                // Closing position
                double close_qty = std::min(std::abs(current_pos), std::abs(qty));
                double pnl = close_qty * (trade.side == OrderSide::SELL
                    ? (trade.price - avg_price[trade.symbol])
                    : (avg_price[trade.symbol] - trade.price));
                realized_pnl += pnl;
                quantity[trade.symbol] += qty;
                if (quantity[trade.symbol] == 0) avg_price[trade.symbol] = 0;
            } else {
                // Opening/adding to position
                double new_qty = quantity[trade.symbol] + qty;
                if (new_qty != 0) {
                    avg_price[trade.symbol] = (avg_price[trade.symbol] * quantity[trade.symbol] + trade.price * qty) / new_qty;
                }
                quantity[trade.symbol] = new_qty;
            }
        }

        return realized_pnl;
    }

private:
    std::unordered_map<std::string, double> positions;
    std::vector<Trade> trades;
};
