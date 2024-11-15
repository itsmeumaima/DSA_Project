//#ifndef ORDERBOOK_H
//#define ORDERBOOK_H
//
//#include "LinkedList.h"
//#include "Order.h"
//#include <vector>
//#include <queue>
//#include <stack>
//
//struct UndoOperation {
//    enum OperationType { PLACE, REMOVE } type;
//    Order* order;
//
//    UndoOperation(OperationType t, Order* o) : type(t), order(o) {}
//};
//// Comparator structures
//struct CompareBuy {
//    bool operator()(Order* const& o1, Order* const& o2) {
//        return o1->getPrice() < o2->getPrice(); // Higher price has higher priority
//    }
//};
//
//struct CompareSell {
//    bool operator()(Order* const& o1, Order* const& o2) {
//        return o1->getPrice() > o2->getPrice(); // Lower price has higher priority
//    }
//};
//
//class OrderBook {
//private:
//    // Priority queues for buy and sell orders
//    std::priority_queue<Order*, std::vector<Order*>, CompareBuy> buyOrdersQueue;
//    std::priority_queue<Order*, std::vector<Order*>, CompareSell> sellOrdersQueue;
//
//    LinkedList buyOrders;
//    LinkedList sellOrders;
//    std::vector<Order*> executedTrades;
//
//    std::stack<UndoOperation> undoStack;
//
//    void matchOrders();
//
//public:
//    ~OrderBook(); // Destructor for cleanup
//    void placeOrder(Order* order);
//    void processOrderMatching();
//    void displayOrderBook() const;
//    void displayExecutedTrades() const;
//    void undoLastOperation();
//    void removeOrder(Order* order);
//    void reloadOrdersFromFile();
//};
//
//#endif // ORDERBOOK_H
//
//
//
#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "LinkedList.h"
#include "Order.h"
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>  // Include this for storing order IDs
#include <set>

// Struct to handle undo operations (place/remove)
struct UndoOperation {
    enum OperationType { PLACE, REMOVE } type;
    Order* order;

    UndoOperation(OperationType t, Order* o) : type(t), order(o) {}
};

// Comparator structures for Buy and Sell Orders
struct CompareBuy {
    bool operator()(Order* const& o1, Order* const& o2) {
        return o1->getPrice() < o2->getPrice(); // Higher price has higher priority
    }
};

struct CompareSell {
    bool operator()(Order* const& o1, Order* const& o2) {
        return o1->getPrice() > o2->getPrice(); // Lower price has higher priority
    }
};

class OrderBook {
private:
    // Priority queues for buy and sell orders
    std::priority_queue<Order*, std::vector<Order*>, CompareBuy> buyOrdersQueue;
    std::priority_queue<Order*, std::vector<Order*>, CompareSell> sellOrdersQueue;

    LinkedList buyOrders;
    LinkedList sellOrders;
    std::vector<Order*> executedTrades;

    std::stack<UndoOperation> undoStack;

    std::set<int> orderIds;  // Set to track order IDs to check for duplicates

    void matchOrders();

public:
    ~OrderBook(); // Destructor for cleanup

    void placeOrder(Order* order);   // Function to place an order
    void processOrderMatching();     // Process order matching
    void displayOrderBook() const;   // Display order book
    void displayExecutedTrades() const;  // Display executed trades
    void undoLastOperation();       // Undo the last operation
    void removeOrder(Order* order); // Remove an order from the book
    void reloadOrdersFromFile();    // Reload orders from file

    bool isOrderIdDuplicate(int orderId);  // Function to check for duplicate order ID
};

#endif // ORDERBOOK_H
