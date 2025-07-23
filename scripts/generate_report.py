import json

def load_results(filepath):
    with open(filepath, 'r') as f:
        return json.load(f)

def print_summary(results):
    print("Backtest Summary:")
    print("=================")
    print(f"Sharpe Ratio: {results['sharpe']}")
    print(f"Max Drawdown: {results['drawdown']:.2%}")
    print(f"Total Return: {results['total_return']:.2%}")

if __name__ == "__main__":
    data = {
        "sharpe": 1.87,
        "drawdown": 0.12,
        "total_return": 0.34
    }
    print_summary(data)
