# Live Strategy Backtester (CLI Edition)

This is a high-performance C++17/C++20-based backtesting engine designed for quantitative trading strategy development, testing, and live shadow simulation — all fully controlled via the terminal.

## 📦 Features

- ✅ Strategy framework with SMA crossover example
- ✅ Multi-asset backtesting support
- ✅ Trade simulation with slippage and latency modeling
- ✅ Portfolio tracking and realized P&L
- ✅ CLI interface to run everything from terminal
- ✅ Trade logs, summaries, and analysis views

---

## 🛠 Build Instructions (macOS/Linux)

```bash
cd live_backtester_cli_enabled
mkdir build
cd build
cmake ..
make
```

You will get the `./backtester` binary inside the `build/` folder.

---

## 🚀 CLI Usage

```bash
./backtester [OPTIONS]
```

### Options:

| Flag | Description |
|------|-------------|
| `--file <path>`       | Path to CSV input (default: `data/sample_ticks.csv`) |
| `--symbols s1,s2,...` | Comma-separated list of asset symbols |
| `--strategy <name>`   | Strategy to use (default: `sma`) |
| `--trades`            | Print trade log to terminal |
| `--pnl`               | Print total realized P&L |
| `--summary`           | Print final portfolio positions |
| `--help`              | Show usage help |

---

## 📁 Example Input (CSV Format)

```csv
timestamp,symbol,price,volume
2024-01-01T09:01:00Z,BTCUSDT,101.00,1.0
2024-01-01T09:02:00Z,BTCUSDT,102.00,1.0
...
```

Use `data/sample_ticks_with_trades.csv` (included) to trigger SMA crossovers.

---

## 🧪 Example Commands

### Run full backtest
```bash
./backtester --file ../data/sample_ticks_with_trades.csv --symbols BTCUSDT --trades --pnl --summary
```

### View help
```bash
./backtester --help
```

---

## 📌 Strategy Notes

- **SMA Strategy**: Buys when short-term SMA crosses above long-term SMA; sells when it crosses below.
- Default windows: `short = 5`, `long = 20`
- Includes slippage model and latency simulation

---

## ✅ What's Next

- [ ] `--export` to save trade logs
- [ ] Metrics like Sharpe, Sortino, Drawdown
- [ ] Walk-forward testing
- [ ] Real-time market feed integration (Binance/OKX)

---


