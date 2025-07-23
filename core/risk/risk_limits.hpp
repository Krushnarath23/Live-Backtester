#pragma once
#include <string>
#include <unordered_map>

class RiskLimits {
public:
    void set_max_position(const std::string& symbol, double limit) {
        max_position[symbol] = limit;
    }

    bool check_position(const std::string& symbol, double current_position) {
        return std::abs(current_position) <= max_position[symbol];
    }

private:
    std::unordered_map<std::string, double> max_position;
};
