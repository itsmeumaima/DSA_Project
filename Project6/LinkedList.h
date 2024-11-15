#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Order.h"

struct LinkedListNode {
    Order* order;
    LinkedListNode* next;
    LinkedListNode(Order* order) : order(order), next(nullptr) {}
};

class LinkedList {
public:
    LinkedListNode* head;
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    void insertOrder(Order* order);
    void removeOrders(Order* order);
    void displayOrders() const;
    void clear();
    void removeOrderFromFile(Order* order);
    //std::string formatTimestamp(const std::time_t& time);
};

#endif // LINKEDLIST_H
