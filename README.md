Built a stock trading simulation project in C++.

This project simulates stock price updates while allowing users to buy, sell, and view thier portfolio.
## Featues:
- **Live Price Updates** – Stock prices change dynamically in the background using multithreading.
- **Buy & Sell Stocks** – Manage a portfolio and execute trades.
- **Thread-Safe Operations** – Uses `std::mutex` to prevent data races.
- **OOP Design** – Classes for `Stock`, `Market`, `User`, and `TradingSystem`.

## How It Works:
1. **Market thread** continuously updates stock prices in the background.
2. **User menu** lets you:
   - View available stocks
   - Buy stocks
   - Sell stocks
   - View your portfolio
  
You can run this code on online compiler such as GDB compiler.


  
