#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Order.h"

struct LinkedListNode {
    Order* order;   //create an object of order type
    LinkedListNode* next;   //pointer to point to the address of next node
    LinkedListNode(Order* order) : order(order), next(nullptr) {}
};

class LinkedList {
public:
    LinkedListNode* head;   //create an object of structure LinkedListNode name head
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    void insertOrder(Order* order);
    void removeOrders(Order* order);
    void displayOrders() const;
    void removeOrderFromFile(Order* order);//after the order being removed then it must have to remove it from the text file

};

#endif // LINKEDLIST_H
