#pragma once
#include <chrono>
#include <random>

class LatencySimulator {
public:
    LatencySimulator(int mean_ms = 10, int stddev_ms = 3)
        : generator(std::random_device{}()), dist(mean_ms, stddev_ms) {}

    std::chrono::system_clock::time_point apply_latency(std::chrono::system_clock::time_point ts) {
        int latency = std::max(0, static_cast<int>(dist(generator)));
        return ts + std::chrono::milliseconds(latency);
    }

private:
    std::mt19937 generator;
    std::normal_distribution<> dist;
};
