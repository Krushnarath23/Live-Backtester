#include "core/engine/backtest_engine.hpp"
#include "core/strategy/strategy_factory.hpp"
#include "core/market/historical_data_loader.hpp"
#include "core/execution/portfolio.hpp"
#include "core/utils/logging.hpp"
#include "core/analytics/performance_metrics.hpp"
#include "core/utils/time_utils.hpp"

#include <sstream>
#include <unordered_map>
#include <getopt.h>

void print_trades(const std::vector<Trade>& trades) {
    std::cout << "\nTrade Log:\n===========\n";
    for (const auto& t : trades) {
        std::cout << (t.side == OrderSide::BUY ? "BUY  " : "SELL ")
                  << t.symbol << " | "
                  << "Qty: " << t.quantity << " | "
                  << "Price: " << t.price << " | "
                  << "Time: " << format_time(t.timestamp)
                  << "\n";
    }
}

void print_help() {
    std::cout << "Usage: ./backtester [OPTIONS]\n"
              << "Options:\n"
              << "  --file <path>         Path to CSV data file\n"
              << "  --symbols <s1,s2>     Comma-separated list of symbols (e.g. BTCUSDT,ETHUSDT)\n"
              << "  --strategy <name>     Strategy name to apply (default: sma)\n"
              << "  --pnl                 Print realized P&L\n"
              << "  --trades              Print trade log\n"
              << "  --summary             Print portfolio summary\n"
              << "  --help                Show this message\n";
}

int main(int argc, char* argv[]) {
    std::string filepath = "data/sample_ticks.csv";
    std::vector<std::string> symbols = {"BTCUSDT"};
    std::string strategy_name = "sma";
    bool show_trades = false, show_pnl = false, show_summary = false;

    static struct option long_options[] = {
        {"file",     required_argument, 0,  'f' },
        {"symbols",  required_argument, 0,  's' },
        {"strategy", required_argument, 0,  't' },
        {"pnl",      no_argument,       0,  'p' },
        {"trades",   no_argument,       0,  'l' },
        {"summary",  no_argument,       0,  'm' },
        {"help",     no_argument,       0,  'h' },
        {0,          0,                 0,   0  }
    };

    int opt;
    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "f:s:t:plmh", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'f': filepath = optarg; break;
            case 's': {
                std::stringstream ss(optarg);
                std::string token;
                symbols.clear();
                while (std::getline(ss, token, ',')) symbols.push_back(token);
                break;
            }
            case 't': strategy_name = optarg; break;
            case 'p': show_pnl = true; break;
            case 'l': show_trades = true; break;
            case 'm': show_summary = true; break;
            case 'h': print_help(); return 0;
            default: print_help(); return 1;
        }
    }

    Logger::log(Logger::Level::INFO, "Loading data from: " + filepath);
    auto ticks = HistoricalDataLoader::load_from_csv(filepath);

    std::unordered_map<std::string, std::unique_ptr<IStrategy>> strategies;
    for (const auto& sym : symbols) {
        strategies[sym] = create_strategy(strategy_name);
    }

    Portfolio port;
    BacktestEngine engine(std::move(strategies), port);
    engine.run(ticks);

    if (show_summary) {
        for (const auto& [sym, qty] : port.all_positions()) {
            Logger::log(Logger::Level::INFO, "Final position in " + sym + ": " + std::to_string(qty));
        }
    }

    if (show_pnl) {
        double pnl = port.calculate_realized_pnl();
        Logger::log(Logger::Level::INFO, "Total Realized P&L: " + std::to_string(pnl));
    }

    if (show_trades) {
        print_trades(port.get_trades());
    }

    return 0;
}
