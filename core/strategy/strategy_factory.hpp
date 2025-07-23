#pragma once
#include "istrategy.hpp"
#include "core/strategy/built_in/sma_crossover.cpp"
#include <memory>
#include <string>

std::unique_ptr<IStrategy> create_strategy(const std::string& name) {
    if (name == "sma") {
        return std::make_unique<SmaCrossover>(5, 20, "BTCUSDT");
    }
    return nullptr;
}
