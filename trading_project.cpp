#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

std::mutex mtx; 

class Stock {
public:
    string name;
    string symbol;
    double price;

    Stock(string n, string s, double p) : name(n), symbol(s), price(p) {}

    void updatePrice() {
        double change = ((rand() % 2001) - 1000) / 100.0; // -10.0 to +10.0
        price += change;
        if (price < 1) price = 1;
    }

    void display() {
        cout << symbol << " - " << name << " : $" << price << endl;
    }
};

class Market {
    vector<Stock> stocks;

public:
    Market() {
        stocks.push_back(Stock("Apple", "AAPL", 150.0));
        stocks.push_back(Stock("Tesla", "TSLA", 700.0));
        stocks.push_back(Stock("Google", "GOOGL", 2800.0));
    }

    void updatePrices() {
        for (size_t i = 0; i < stocks.size(); ++i) {
            stocks[i].updatePrice();
        }
    }

    void showStocks() {
        for (size_t i = 0; i < stocks.size(); ++i) {
            stocks[i].display();
        }
    }

    Stock* getStockBySymbol(string symbol) {
        for (size_t i = 0; i < stocks.size(); ++i) {
            if (stocks[i].symbol == symbol)
                return &stocks[i];
        }
        return nullptr;
    }
};

class User {
    double cash;
    map<string, int> portfolio;

public:
    User() : cash(10000.0) {}

    void buyStock(Stock* stock, int quantity) {
        double totalCost = stock->price * quantity;
        if (totalCost > cash) {
            cout << "Not enough cash!\n";
        } else {
            cash -= totalCost;
            portfolio[stock->symbol] += quantity;
            cout << "Bought " << quantity << " of " << stock->symbol << "\n";
        }
    }

    void sellStock(Stock* stock, int quantity) {
        if (portfolio[stock->symbol] < quantity) {
            cout << "Not enough shares!\n";
        } else {
            cash += stock->price * quantity;
            portfolio[stock->symbol] -= quantity;
            cout << "Sold " << quantity << " of " << stock->symbol << "\n";
        }
    }

    void viewPortfolio(Market& market) {
        cout << "\nPortfolio:\n";
        for (map<string, int>::iterator it = portfolio.begin(); it != portfolio.end(); ++it) {
            string symbol = it->first;
            int quantity = it->second;
            Stock* stock = market.getStockBySymbol(symbol);
            if (stock)
                cout << symbol << " : " << quantity << " shares, Price: $" << stock->price << endl;
        }
        cout << "Cash: $" << cash << "\n";
    }
};

class TradingSystem {
    Market market;
    User user;
    bool running;

public:
    TradingSystem() : running(true) {}

    void priceUpdater() {
        while (running) {
            std::this_thread::sleep_for(chrono::seconds(5));
            if (mtx.try_lock()) {
                market.updatePrices();
                cout << "\n[Prices auto-updated]\n";
                mtx.unlock();
            }
        }
    }

    void run() {
        srand(time(0));
        std::thread priceThread(&TradingSystem::priceUpdater, this);

        int choice;
        while (true) {
            cout << "\n--- Trading System ---\n";
            cout << "1. Show Stocks\n2. Buy Stock\n3. Sell Stock\n4. View Portfolio\n5. Exit\n";
            cout << "Choice: ";
            cin >> choice;

            mtx.lock();

            if (choice == 1) {
                market.showStocks();
            } else if (choice == 2) {
                string symbol;
                int qty;
                cout << "Enter symbol: ";
                cin >> symbol;
                cout << "Enter quantity: ";
                cin >> qty;
                Stock* stock = market.getStockBySymbol(symbol);
                if (stock) user.buyStock(stock, qty);
                else cout << "Stock not found!\n";
            } else if (choice == 3) {
                string symbol;
                int qty;
                cout << "Enter symbol: ";
                cin >> symbol;
                cout << "Enter quantity: ";
                cin >> qty;
                Stock* stock = market.getStockBySymbol(symbol);
                if (stock) user.sellStock(stock, qty);
                else cout << "Stock not found!\n";
            } else if (choice == 4) {
                user.viewPortfolio(market);
            } else if (choice == 5) {
                running = false;
                mtx.unlock();
                break;
            } else {
                cout << "Invalid choice!\n";
            }

            mtx.unlock();
        }

        priceThread.join();
    }
};

int main() {
    TradingSystem system;
    system.run();
    return 0;
}
