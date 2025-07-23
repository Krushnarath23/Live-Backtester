#pragma once
#include <string>

enum class OrderType { MARKET, LIMIT };
enum class OrderSide { BUY, SELL };

struct Order {
    std::string symbol;
    OrderType type;
    OrderSide side;
    double price;
    double quantity;
    std::chrono::system_clock::time_point timestamp;
};
