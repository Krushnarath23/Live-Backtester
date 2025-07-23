#pragma once
#include <vector>
#include <cmath>
#include <numeric>

class PerformanceMetrics {
public:
    static double sharpe_ratio(const std::vector<double>& returns, double risk_free_rate = 0.0) {
        if (returns.empty()) return 0.0;

        double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
        double sum_sq_diff = 0.0;
        for (auto r : returns) sum_sq_diff += (r - mean) * (r - mean);
        double std_dev = std::sqrt(sum_sq_diff / returns.size());
        return std_dev > 0 ? (mean - risk_free_rate) / std_dev : 0.0;
    }
};
