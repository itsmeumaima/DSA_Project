#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "LinkedList.h"
#include "Order.h"
#include <vector>
#include <queue>
#include <set>
using namespace std;
#include "Stack.h" 

class UndoOperation {
public:
    enum Type { PLACE, REMOVE };
    Type type;
    Order* order;

    UndoOperation(Type type, Order* order) : type(type), order(order) {}
    UndoOperation() : type(PLACE), order(nullptr) {}
};

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
    priority_queue<Order*, vector<Order*>, CompareBuy> buyOrdersQueue;
    priority_queue<Order*, vector<Order*>, CompareSell> sellOrdersQueue;

    LinkedList buyOrders;
    LinkedList sellOrders;
    vector<Order*> executedTrades;
    Stack<UndoOperation> undoStack;

    set<int> orderIds;

    void matchOrders();

public:
    void removeOrder(Order* order);
    string toUpperCase(const string& str);
    void placeOrder(Order* order);
    void processOrderMatching();
    void displayOrderBook() const;
    void displayExecutedTrades() const;
    void undoLastOperation();
    void addOrdertoFile(Order* order);
    bool isOrderIdDuplicate(int orderId);
    void updateOrderInFile(Order* order);
};
#include "Stack.tpp" // Include the template implementation file

#endif // ORDERBOOK_H
