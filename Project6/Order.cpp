#include "Order.h"
using namespace std;

Order::Order(int id, const string& stockSymbol, double price, int quantity, OrderType type)
    : id(id), stockSymbol(stockSymbol), price(price), quantity(quantity), type(type), timestamp(time(nullptr)) {}
    //timestamp(std::time(nullptr)): This initializes the timestamp member to the current time when the order is created.

void Order::displayOrder() const {
    cout << "Order ID: " << id
        << ", Stock Symbol: " << stockSymbol
        << ", Quantity: " << quantity
        << ", Price: " << price
        << ", Type: " << (type == BUY ? "Buy" : "Sell");

    //The below code (which represents the number of seconds since the Unix epoch) into a human - readable date and time format.
    char buffer[26];  // Declare a character array to hold the formatted timestamp
    ctime_s(buffer, sizeof(buffer), &timestamp); // Convert time_t to a string in a safe way
    buffer[24] = '\0'; // Remove the newline character added by ctime_s
    cout << ", Timestamp: " << buffer << endl; // Output the timestamp
}

// Getter methods
int Order::getId() const {
    return id;
}

string Order::getStockSymbol() const {
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

time_t Order::getTimestamp() const {
    return timestamp;
}
// reduces the quantity after trade being executed
void Order::reduceQuantity(int qty) {
    quantity -= qty;
}  
void Order::setPrice(double newPrice) {
    price = newPrice;
}

