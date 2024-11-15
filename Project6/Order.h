#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ctime>
#include <iostream>

enum OrderType { BUY, SELL };

class Order {
public:
    int id;
    std::string stockSymbol;
    double price;
    int quantity;
    OrderType type;
    std::time_t timestamp;
    std::string tif; // Time in Force as a string

    Order(int id, const std::string& stockSymbol, double price, int quantity, OrderType type, const std::string& tif);
    void displayOrder() const;

    // Getter methods
    int getId() const;
    std::string getStockSymbol() const;
    double getPrice() const;
    int getQuantity() const;
    OrderType getOrderType() const;
    std::time_t getTimestamp() const;
    std::string getTif() const;

    // Method to reduce quantity
    void reduceQuantity(int amount);
};

#endif // ORDER_H
