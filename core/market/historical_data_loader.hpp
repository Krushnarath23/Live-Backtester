#pragma once
#include "tick_data.hpp"
#include <vector>
#include <fstream>
#include <sstream>

class HistoricalDataLoader {
public:
    static std::vector<TickData> load_from_csv(const std::string& filename) {
        std::vector<TickData> data;
        std::ifstream file(filename);
        std::string line;

        // ✅ Skip the header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string token;
            TickData tick;

            // Skip timestamp (we don't parse it here)
            std::getline(ss, token, ',');

            std::getline(ss, tick.symbol, ',');

            std::getline(ss, token, ',');
            try {
                tick.price = std::stod(token);
            } catch (...) {
                continue;
            }

            std::getline(ss, token, ',');
            try {
                tick.volume = std::stod(token);
            } catch (...) {
                continue;
            }

            tick.timestamp = std::chrono::system_clock::now(); // placeholder

            data.push_back(tick);
        }

        return data;
    }
};
