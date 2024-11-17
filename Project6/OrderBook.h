#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "LinkedList.h"
#include "Order.h"
#include <vector>
#include <queue>
#include <stack>
#include <set>
using namespace std;

// Struct to handle undo operations (place/remove)
struct UndoOperation {
    enum OperationType { PLACE, REMOVE } type;
    Order* order;

    UndoOperation(OperationType t, Order* o) : type(t), order(o) {}
};

// This comparator ensures that the highest price buy orders are given the highest priority in the buy orders queue.
struct CompareBuy {
    bool operator()(Order* const& o1, Order* const& o2) {
        return o1->getPrice() < o2->getPrice(); // Higher price has higher priority
    }
};

//This comparator ensures that the lowest price sell orders are given the highest priority in the sell orders queue
struct CompareSell {
    bool operator()(Order* const& o1, Order* const& o2) {
        return o1->getPrice() > o2->getPrice(); // Lower price has higher priority
    }
};

class OrderBook {
private:
    // Priority queues for buy and sell orders
    priority_queue<Order*,vector<Order*>, CompareBuy> buyOrdersQueue;
    priority_queue<Order*,vector<Order*>, CompareSell> sellOrdersQueue;

    LinkedList buyOrders;//make a seperate object of linkedList class for buy orders
    LinkedList sellOrders;//make a seperate object of linkedlist class for sell orders
    vector<Order*> executedTrades;//a vector to store executed trade

    stack<UndoOperation> undoStack; //A stack to store operations that can be undone(place / remove).

    set<int> orderIds;  // Set to track order IDs to check for duplicates

    void matchOrders();

public:

    string toUpperCase(const string& str);
    void placeOrder(Order* order);   // Function to place an order
    void processOrderMatching();     // Process order matching
    void displayOrderBook() const;   // Display order book
    void displayExecutedTrades() const;  // Display executed trades
    void undoLastOperation();       // Undo the last operation
    void removeOrder(Order* order); // Remove an order from the book
    void addOrdertoFile(Order* order);
    bool isOrderIdDuplicate(int orderId);  // Function to check for duplicate order ID
    void updateOrderInFile(Order* order);
};

#endif // ORDERBOOK_H
