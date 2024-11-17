#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ctime>
#include <iostream>
using namespace std;
//enum is a user-defined type consisting of a set of named integer constants.By default,
// in C++, the first constant (BUY) is assigned the value 0, and the second constant (SELL) 
// is assigned the value 1
enum OrderType { BUY, SELL };

class Order {
public:
    int id;
    string stockSymbol;
    double price;
    int quantity;
    OrderType type;//This declares a variable type of the enumeration type OrderType
    time_t timestamp;

    Order(int id, const std::string& stockSymbol, double price, int quantity, OrderType type);
    void displayOrder() const;

    // Getter methods
    int getId() const;
    string getStockSymbol() const;
    double getPrice() const;
    int getQuantity() const;
    OrderType getOrderType() const;
    time_t getTimestamp() const;

    // Method to reduce quantity after matching and any trade executed
    void reduceQuantity(int amount);
};

#endif // ORDER_H
