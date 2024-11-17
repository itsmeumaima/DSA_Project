#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "OrderBook.h"
#include "Order.h"
using namespace std;

void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Place a new order\n";
    cout << "2. Display current order book\n";
    cout << "3. Process order matching\n";
    cout << "4. Display executed trades\n";
    cout << "5. Undo last operation\n";  
    cout << "6. Exit\n";
    cout << "Choose an option: ";
}

void placeNewOrder(OrderBook& orderBook) {
    int id, quantity;
    double price;
    string symbol, orderType;

    cout << "Enter Order ID: ";
    cin >> id;

    if (orderBook.isOrderIdDuplicate(id)) {
        cout << "Error: Order ID " << id << " already exists. Please enter a unique ID.\n";
        return ;  // Exit early if the order ID is a duplicate
    }
    cout << "Enter Symbol (e.g., AAPL): ";
    cin >> symbol;
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Order Type (BUY/SELL): ";
    cin >> orderType;
    symbol=orderBook.toUpperCase(symbol);
    orderType = orderBook.toUpperCase(orderType);

    Order* newOrder = new Order(id, symbol, price, quantity,
        (orderType == "BUY" ? BUY : SELL));
    orderBook.placeOrder(newOrder);

    // Append the new order to the file
    ofstream outfile;
    outfile.open("Data1.txt", ios::app);
    if (outfile.is_open()) {
        //outfile << "\n" << id << ","
        outfile<<id<<","
            << symbol  << ","
            << price << ","
            << quantity << ","
            << orderType << "\n";
        outfile.close();
        cout << "Order placed successfully.\n";
    }
    else {
        cerr << "Unable to open the file for writing.\n";
    }
}
int main() {

    cout << "------------WELCOME TO THE STOCK MARKET ORDER BOOK APPLICATION-----------------" << "\n\n";
    OrderBook orderBook;
    fstream myfile;
    myfile.open("Data1.txt", ios::in);  // Open the file in read mode
    if (myfile.is_open()) {
        string line;
        while (getline(myfile, line)) {
            stringstream ss(line);
            int id;
            string symbol, actionStr;
            double price;
            int quantity;

            // Parse the line into individual values using comma as delimiter
            ss >> id; ss.ignore();            // ID (1)
            getline(ss, symbol, ',');    // Symbol (AAPL)
            ss >> price; ss.ignore();         // Price (150.00)
            ss >> quantity; ss.ignore();      // Quantity (100)
            getline(ss, actionStr, ','); // Action (BUY)            

            // Remove any leading or trailing whitespace from the symbol and action
            symbol.erase(0, symbol.find_first_not_of(" \t\n\r\f\v"));
            symbol.erase(symbol.find_last_not_of(" \t\n\r\f\v") + 1);
            actionStr.erase(0, actionStr.find_first_not_of(" \t\n\r\f\v"));
            actionStr.erase(actionStr.find_last_not_of(" \t\n\r\f\v") + 1);

            symbol = orderBook.toUpperCase(symbol);
            actionStr = orderBook.toUpperCase(actionStr);
            // Convert action string to OrderType enum
            OrderType action = (actionStr == "BUY") ? BUY : SELL;

            // Create an Order object
            Order* neworder = new Order(id, symbol, price, quantity, action);
            orderBook.placeOrder(neworder);
        }
        myfile.close();  // Close the file after reading
    }
    else {
        cout << "Unable to open the file!" <<endl;
    }

    int choice;
    bool exit = false;

    while (!exit) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            placeNewOrder(orderBook);  // Let the user place a new order
            break;
        case 2:
            orderBook.displayOrderBook();
            break;
        case 3:
            orderBook.processOrderMatching();
            break;
        case 4:
            orderBook.displayExecutedTrades();
            break;
        case 5:
            orderBook.undoLastOperation();  // Call undo
            break;
        case 6:
            exit = true;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
            break;
        }
    }

    return 0;
}
