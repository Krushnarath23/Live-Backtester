#pragma once
#include "order.hpp"

class SlippageModel {
public:
    // Fixed slippage (e.g., 0.05 means 5 cents slippage per trade)
    explicit SlippageModel(double fixed_slippage = 0.05)
        : slippage(fixed_slippage) {}

    double apply(const Order& order) const {
        if (order.side == OrderSide::BUY) {
            return order.price + slippage;
        } else {
            return order.price - slippage;
        }
    }

private:
    double slippage;
};
