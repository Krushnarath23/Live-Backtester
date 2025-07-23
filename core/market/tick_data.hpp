#pragma once
#include <chrono>
#include <string>

struct TickData {
    std::chrono::system_clock::time_point timestamp;
    std::string symbol;
    double price;
    double volume;
};
