#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "OrderBook.h"
#include "Order.h"

void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Place a new order\n";
    std::cout << "2. Display current order book\n";
    std::cout << "3. Process order matching\n";
    std::cout << "4. Display executed trades\n";
    std::cout << "5. Undo last operation\n";  // New option for undo
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

void placeNewOrder(OrderBook& orderBook) {
    int id, quantity;
    double price;
    std::string symbol, orderType;

    std::cout << "Enter Order ID: ";
    std::cin >> id;

    if (orderBook.isOrderIdDuplicate(id)) {
        std::cout << "Error: Order ID " << id << " already exists. Please enter a unique ID.\n";
        return ;  // Exit early if the order ID is a duplicate
    }

    /*if (orderBook.isOrderIdDuplicate(id)) {
        std::cout << "Error: Order ID " << id << " already exists.\n";
        return;
    }*/

    std::cout << "Enter Symbol (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter Price: ";
    std::cin >> price;
    std::cout << "Enter Quantity: ";
    std::cin >> quantity;
    std::cout << "Enter Order Type (BUY/SELL): ";
    std::cin >> orderType;

    Order* newOrder = new Order(id, symbol, price, quantity,
        (orderType == "BUY" ? BUY : SELL));
    orderBook.placeOrder(newOrder);

    // Append the new order to the file
    std::ofstream outfile;
    outfile.open("Data1.txt", std::ios::app);
    if (outfile.is_open()) {
        outfile << "\n" << id << ","
            << '"' << symbol << '"' << ","
            << price << ","
            << quantity << ","
            << orderType << ","<< "\n";
        outfile.close();
        std::cout << "Order placed successfully and added to the file.\n";
    }
    else {
        std::cerr << "Unable to open the file for writing.\n";
    }
}

int main() {
    OrderBook orderBook;
    std::fstream myfile;
    myfile.open("Data1.txt", std::ios::in);  // Open the file in read mode
    if (myfile.is_open()) {
        std::string line;
        while (std::getline(myfile, line)) {
            std::stringstream ss(line);
            int id;
            std::string symbol, actionStr;
            double price;
            int quantity;

            // Parse the line into individual values using comma as delimiter
            ss >> id; ss.ignore();            // ID (1)
            std::getline(ss, symbol, ',');    // Symbol (AAPL)
            ss >> price; ss.ignore();         // Price (150.00)
            ss >> quantity; ss.ignore();      // Quantity (100)
            std::getline(ss, actionStr, ','); // Action (BUY)

            //if (orderBook.isOrderIdDuplicate(id)) {
            //    std::cout << "Error: Order ID " << id << " already exists. Please enter a unique ID.\n";
            //    break;  // Exit early if the order ID is a duplicate
            //}

            // Remove any leading or trailing whitespace from the symbol and action
            symbol.erase(0, symbol.find_first_not_of(" \t\n\r\f\v"));
            symbol.erase(symbol.find_last_not_of(" \t\n\r\f\v") + 1);
            actionStr.erase(0, actionStr.find_first_not_of(" \t\n\r\f\v"));
            actionStr.erase(actionStr.find_last_not_of(" \t\n\r\f\v") + 1);

            // Convert action string to OrderType enum
            OrderType action = (actionStr == "BUY") ? BUY : SELL;

            // Create an Order object
            Order* neworder = new Order(id, symbol, price, quantity, action);
            orderBook.placeOrder(neworder);
        }
        myfile.close();  // Close the file after reading
    }
    else {
        std::cout << "Unable to open the file!" << std::endl;
    }

    int choice;
    bool exit = false;

    while (!exit) {
        displayMenu();
        std::cin >> choice;

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
            std::cout << "Invalid option. Please try again.\n";
            break;
        }
    }

    return 0;
}
