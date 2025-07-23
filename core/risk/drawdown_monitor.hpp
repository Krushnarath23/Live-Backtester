#pragma once
#include <limits>

class DrawdownMonitor {
public:
    void update(double equity) {
        peak = std::max(peak, equity);
        double dd = (peak - equity) / peak;
        max_drawdown = std::max(max_drawdown, dd);
    }

    double get_max_drawdown() const {
        return max_drawdown;
    }

private:
    double peak = std::numeric_limits<double>::lowest();
    double max_drawdown = 0.0;
};
