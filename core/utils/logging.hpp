#pragma once
#include <iostream>
#include <string>

namespace Logger {
    enum class Level { INFO, WARNING, ERROR };

    inline void log(Level level, const std::string& msg) {
        switch (level) {
            case Level::INFO: std::cout << "[INFO] " << msg << std::endl; break;
            case Level::WARNING: std::cout << "[WARNING] " << msg << std::endl; break;
            case Level::ERROR: std::cerr << "[ERROR] " << msg << std::endl; break;
        }
    }
}
