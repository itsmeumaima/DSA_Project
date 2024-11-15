#include "OrderBook.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

void OrderBook::reloadOrdersFromFile() {
    std::ifstream infile("Data1.txt");
    if (infile.is_open()) {
        std::string line;
        buyOrders.clear();  // Clear current orders before reloading
        sellOrders.clear();

        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            int id;
            std::string symbol, orderType, timeInForce;
            double price;
            int quantity;

            ss >> id; ss.ignore();
            std::getline(ss, symbol, ',');
            ss >> price; ss.ignore();
            ss >> quantity; ss.ignore();
            std::getline(ss, orderType, ',');
            std::getline(ss, timeInForce);

            Order* newOrder = new Order(id, symbol, price, quantity,
                (orderType == "BUY" ? BUY : SELL), timeInForce);
            if (orderType == "BUY") {
                buyOrders.insertOrder(newOrder);
            }
            else {
                sellOrders.insertOrder(newOrder);
            }
        }
        infile.close();
    }
    else {
        std::cerr << "Error opening file for reloading orders." << std::endl;
    }
}

void OrderBook::placeOrder(Order* order) {
    if (order->getOrderType() == BUY) {
        buyOrders.insertOrder(order);
        buyOrdersQueue.push(order);
    }
    else {
        sellOrders.insertOrder(order);
        sellOrdersQueue.push(order);
    }
    undoStack.push(UndoOperation(UndoOperation::PLACE, order));
}

// Remove an order and add the undo operation
void OrderBook::removeOrder(Order* order) {
    if (order->getOrderType() == BUY) {
        /*buyOrdersQueue.remove(order);*/
        buyOrders.removeOrders(order);
    }
    else {
        /*sellOrdersQueue.remove(order);*/
        sellOrders.removeOrders(order);
    }
    // Add the operation to the undo stack
    undoStack.push(UndoOperation(UndoOperation::REMOVE, order));
}

// Undo the last operation (either place or remove)
void OrderBook::undoLastOperation() {
    if (!undoStack.empty()) {
        UndoOperation lastOperation = undoStack.top();
        undoStack.pop();

        if (lastOperation.type == UndoOperation::PLACE) {
            removeOrder(lastOperation.order);
        }
        else if (lastOperation.type == UndoOperation::REMOVE) {
            placeOrder(lastOperation.order);
        }
        std::cout << "Undo operation completed.\n";
        reloadOrdersFromFile();  // Reload the orders from the file
    }
    else {
        std::cout << "No operations to undo.\n";
    }
}



void OrderBook::matchOrders() {
    while (!buyOrdersQueue.empty() && !sellOrdersQueue.empty()) {
        Order* highestBuy = buyOrdersQueue.top();
        Order* lowestSell = sellOrdersQueue.top();

        std::cout << "Trying to match Buy Order ID: " << highestBuy->getId()
            << " (Price: " << highestBuy->getPrice() << ", Quantity: " << highestBuy->getQuantity() << ") "
            << "with Sell Order ID: " << lowestSell->getId()
            << " (Price: " << lowestSell->getPrice() << ", Quantity: " << lowestSell->getQuantity() << ")"
            << std::endl;

        if (highestBuy->getPrice() >= lowestSell->getPrice()) {
            int executedQuantity = std::min(highestBuy->getQuantity(), lowestSell->getQuantity());
            double executedPrice = (highestBuy->getPrice() + lowestSell->getPrice()) / 2.0;

            // Store the executed trade in executedTrades
            executedTrades.push_back(new Order(
                -1,  // Assign a dummy ID or leave as needed
                highestBuy->getStockSymbol(),
                executedPrice,
                executedQuantity,
                BUY,   // Could be adjusted depending on the logic
                "Immediate"  // Dummy time in force
            ));

            // Display executed trade
            std::cout << "Executed Trade: " << executedQuantity
                << " shares at $" << executedPrice
                << " between Buy Order ID: " << highestBuy->getId()
                << " and Sell Order ID: " << lowestSell->getId() << std::endl;

            highestBuy->reduceQuantity(executedQuantity);
            lowestSell->reduceQuantity(executedQuantity);

            if (highestBuy->getQuantity() == 0) {
                buyOrdersQueue.pop();
                buyOrders.removeOrders(highestBuy);
                delete highestBuy;
            }

            if (lowestSell->getQuantity() == 0) {
                sellOrdersQueue.pop();
                sellOrders.removeOrders(lowestSell);
                delete lowestSell;
            }
        }
        else {
            break; // No more matching possible
        }
    }
}


void OrderBook::processOrderMatching() {
    matchOrders();
}

void OrderBook::displayOrderBook() const {
    std::cout << "Buy Orders:" << std::endl;
    buyOrders.displayOrders();
    std::cout << "\n";
    std::cout << "Sell Orders:" << std::endl;
    sellOrders.displayOrders();
}



void OrderBook::displayExecutedTrades() const {
    std::cout << "Executed Trades:" << std::endl;
    for (const auto& trade : executedTrades) {
        trade->displayOrder();
    }
}

OrderBook::~OrderBook() {
    // Clear all buy orders
    
}
