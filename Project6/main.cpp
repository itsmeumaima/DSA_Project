#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>  // For numeric_limits
#include <cctype>  // For toupper
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

bool isAlpha(const string& str) {
    for (char c : str) {
        if (!isalpha(c)) {
            return false;  // Return false if non-alphabetic character is found
        }
    }
    return true;
}

void placeNewOrder(OrderBook& orderBook) {
    int id, quantity;
    double price;
    string symbol, orderType;

    cout << "Enter Order ID: ";
    cin >> id;

    if (orderBook.isOrderIdDuplicate(id)) {
        cout << "Error: Order ID " << id << " already exists. Please enter a unique ID.\n";
        return;  // Exit early if the order ID is a duplicate
    }

    // Error handling for symbol input
    cout << "Enter Symbol (e.g., AAPL): ";
    cin >> symbol;
    while (symbol.empty() || !isAlpha(symbol)) {  // Ensure symbol is alphabetic and not empty
        cout << "Invalid input. Please enter a valid symbol (alphabetic characters only): ";
        cin >> symbol;
    }
    // Convert symbol to uppercase
    for (char& c : symbol) {
        c = toupper(c);
    }

    // Error handling for price input
    cout << "Enter Price: ";
    while (!(cin >> price) || price <= 0) {
        cout << "Invalid input. Please enter a valid price (positive number): ";
        cin.clear();  // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
    }

    // Error handling for quantity input
    cout << "Enter Quantity: ";
    while (!(cin >> quantity) || quantity <= 0) {
        cout << "Invalid input. Please enter a valid quantity (positive number): ";
        cin.clear();  // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
    }

    // Error handling for order type input
    cout << "Enter Order Type (BUY/SELL): ";
    cin >> orderType;
    // Convert order type to uppercase
    for (char& c : orderType) {
        c = toupper(c);
    }
    while (orderType != "BUY" && orderType != "SELL") {
        cout << "Invalid order type. Please enter 'BUY' or 'SELL': ";
        cin >> orderType;
        // Convert to uppercase to avoid case sensitivity issues
        for (char& c : orderType) {
            c = toupper(c);
        }
    }

    // Create new order
    Order* newOrder = new Order(id, symbol, price, quantity,
        (orderType == "BUY" ? BUY : SELL));
    orderBook.placeOrder(newOrder);

    // Append the new order to the file
    ofstream outfile;
    outfile.open("Data1.txt", ios::app);
    if (outfile.is_open()) {
        outfile << id << ","
            << symbol << ","
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

    cout << "\n\n-------------------------------WELCOME TO THE STOCK MARKET ORDER BOOK APPLICATION--------------------------------------- " << "\n";
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

        try {
            switch (choice) {
            case 1:
                placeNewOrder(orderBook);  // Let the user place a new order
                cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                break;
            case 2:
                orderBook.displayOrderBook();
                cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                break;
            case 3:
                orderBook.processOrderMatching();
                cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                break;
            case 4:
                orderBook.displayExecutedTrades();
                cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                break;
            case 5:
                orderBook.undoLastOperation();
                cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                break;
            case 6:
                exit = true;
                break;
            default:
                throw invalid_argument("Invalid option. Please try again.");
            }
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
        }
        catch (const runtime_error& e) {
            cout << "Runtime error: " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "An error occurred: " << e.what() << endl;
        }
    }

    return 0;
}
