#include "Order.h"

Order::Order(int id, const std::string& stockSymbol, double price, int quantity, OrderType type)
    : id(id), stockSymbol(stockSymbol), price(price), quantity(quantity), type(type), timestamp(std::time(nullptr)) {}

void Order::displayOrder() const {
    std::cout << "Order ID: " << id
        << ", Stock Symbol: " << stockSymbol
        << ", Quantity: " << quantity
        << ", Price: " << price
        << ", Type: " << (type == BUY ? "Buy" : "Sell");

    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &timestamp); // Using ctime_s for thread safety
    buffer[24] = '\0'; // Remove newline character
    std::cout << ", Timestamp: " << buffer << std::endl;
}

// Getter methods
int Order::getId() const {
    return id;
}

std::string Order::getStockSymbol() const {
    return stockSymbol;
}

double Order::getPrice() const {
    return price;
}

int Order::getQuantity() const {
    return quantity;
}

OrderType Order::getOrderType() const {
    return type;
}

std::time_t Order::getTimestamp() const {
    return timestamp;
}


void Order::reduceQuantity(int qty) {
    quantity -= qty;
}
