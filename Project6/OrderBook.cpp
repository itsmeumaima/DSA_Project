#include "OrderBook.h"
#include "LinkedList.h"
#include "Order.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void OrderBook::addOrdertoFile(Order* order) {
    ofstream outfile;
    string action = (order->type == 0) ? "BUY" : "SELL";
    outfile.open("Data1.txt", ios::app);
    if (outfile.is_open()) {
        outfile << order->id << ","
            << order->stockSymbol << ","
            << order->price << ","
            << order->quantity << ","
            << action << "\n";
        outfile.close();
    }
    else {
        cerr << "Unable to open the file for writing.\n";
    }
}

string OrderBook::toUpperCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void OrderBook::placeOrder(Order* order) {
    orderIds.insert(order->getId());
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

void OrderBook::removeOrder(Order* order) {
    if (order->getOrderType() == BUY) {
        buyOrders.removeOrders(order); 
        buyOrdersQueue.pop();
    }
    else {
        sellOrders.removeOrders(order);
        buyOrdersQueue.pop();
    }
    undoStack.push(UndoOperation(UndoOperation::REMOVE, order));
}


void removeLastElement(set<int>& orderIds) {
    if (!orderIds.empty()) {
        auto lastElementIt = orderIds.rbegin();
        int lastElement = *lastElementIt;
        orderIds.erase(lastElement);
        std::cout << "Last element (" << lastElement << ") removed from the set.\n";
    }
    else {
        std::cout << "The set is empty. No element to remove.\n";
    }
}

void OrderBook::undoLastOperation() {
    if (!undoStack.isEmpty()) {
        UndoOperation lastOperation = undoStack.pop();
        if (lastOperation.type == UndoOperation::PLACE) {
            removeOrder(lastOperation.order);
            removeLastElement(orderIds);
        }
        else if (lastOperation.type == UndoOperation::REMOVE) {
            placeOrder(lastOperation.order);
            addOrdertoFile(lastOperation.order);
        }
        std::cout << "Undo operation completed.\n";
    }
    else {
        std::cout << "No operations to undo.\n";
    }
}

void OrderBook::updateOrderInFile(Order* order) {
    std::ifstream infile("Data1.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        int id;
        std::string symbol, orderType;
        double price;
        int quantity;

        ss >> id; ss.ignore();
        std::getline(ss, symbol, ',');
        ss >> price; ss.ignore();
        ss >> quantity; ss.ignore();
        std::getline(ss, orderType, ',');

        if (id == order->getId()) {
            quantity = order->getQuantity();
        }

        std::ostringstream updatedLine;
        updatedLine << id << ","
            << symbol << ","
            << price << ","
            << quantity << ","
            << orderType;
        lines.push_back(updatedLine.str());
    }
    infile.close();

    std::ofstream outfile("Data1.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    for (const auto& l : lines) {
        outfile << l << "\n";
    }
    outfile.close();
}
void OrderBook::matchOrders() {
    while (!buyOrdersQueue.empty() && !sellOrdersQueue.empty()) {
        Order* highestBuy = buyOrdersQueue.top();
        Order* lowestSell = sellOrdersQueue.top();
        std::cout << highestBuy->getStockSymbol() << "\n";
        std::cout << lowestSell->getStockSymbol() << "\n";
        if (highestBuy->getStockSymbol() == lowestSell->getStockSymbol()) {
            std::cout << "Trying to match Buy Order ID: " << highestBuy->getId()
                << " (Price: " << highestBuy->getPrice() << ", Quantity: " << highestBuy->getQuantity() << ") "
                << "with Sell Order ID: " << lowestSell->getId()
                << " (Price: " << lowestSell->getPrice() << ", Quantity: " << lowestSell->getQuantity() << ")"
                << endl;

            if (highestBuy->getPrice() >= lowestSell->getPrice()) {
                int executedQuantity = std::min(highestBuy->getQuantity(), lowestSell->getQuantity());
                double executedPrice = (highestBuy->getPrice() + lowestSell->getPrice()) / 2.0;

                // Store the executed trade in executedTrades
                executedTrades.push_back(new Order(
                    -1,  // Assign a dummy ID
                    highestBuy->getStockSymbol(),
                    executedPrice,
                    executedQuantity,
                    BUY
                ));

                // Display executed trade
                std::cout << "Executed Trade: " << executedQuantity
                    << " shares at $" << executedPrice
                    << " between Buy Order ID: " << highestBuy->getId()
                    << " and Sell Order ID: " << lowestSell->getId() << std::endl;

                highestBuy->reduceQuantity(executedQuantity);
                updateOrderInFile(highestBuy);
                lowestSell->reduceQuantity(executedQuantity);
                updateOrderInFile(lowestSell);

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
        else {
            std::cout << "No order matches..." << endl;
            return;
        }
    }
}

void OrderBook::processOrderMatching() {
    matchOrders();
}
bool OrderBook::isOrderIdDuplicate(int orderId) {
    return orderIds.find(orderId) != orderIds.end();
}

void OrderBook::displayOrderBook() const {
    std::cout << "Buy Orders:" << endl;
    buyOrders.displayOrders();
    std::cout << "\n";
    std::cout << "Sell Orders:" << endl;
    sellOrders.displayOrders();
}

void OrderBook::displayExecutedTrades() const {
    std::cout << "Executed Trades:" << std::endl;
    if (executedTrades.empty()) {
        std::cout << "No recent trade executed.\n";
        return;
    }
    for (const auto& trade : executedTrades) {
        std::cout << "Trade ID: " << trade->getId()
            << ", Symbol: " << trade->getStockSymbol()
            << ", Price: $" << trade->getPrice()
            << ", Quantity: " << trade->getQuantity()
            << std::endl;
    }
}
