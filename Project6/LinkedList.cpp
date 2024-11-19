#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>  // For std::remove and std::rename
using namespace std;

void LinkedList::removeOrderFromFile(Order* order) {
    ifstream infile("Data1.txt");
    ofstream tempfile("temp.txt");//Opens (or creates) a temporary file temp.txt for writing the updated content.

    if (infile.is_open() && tempfile.is_open()) {
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            int id;
            string symbol, orderType, timeInForce;
            double price;
            int quantity;

            ss >> id; ss.ignore();//ss.ignore() is used to skip over commas 
            getline(ss, symbol, ',');
            ss >> price; ss.ignore();
            ss >> quantity; ss.ignore();
            getline(ss, orderType, ',');
            getline(ss, timeInForce);

            // If the order ID matches the one to remove, skip writing it to the file
            if (id != order->getId()) {
                tempfile << line << endl;
            }
        }

        infile.close();
        tempfile.close();

        // Replace the original file with the updated file
        if (remove("Data1.txt") != 0) {    //The original file Data1.txt is deleted using std::remove().
            cerr << "Error deleting the original file.\n";
        }

        if (rename("temp.txt", "Data1.txt") != 0) {
            cerr << "Error renaming the temporary file.\n";
        }
    }
    else {
        cerr << "Unable to open the file for updating.\n";
    }
}

// Helper function to format time manually
std::string formatTimestamp(const std::time_t& time) {
    std::tm tm;
    localtime_s(&tm, &time); // Using localtime_s for thread safety
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", &tm);
    return std::string(buffer);
}

LinkedList::~LinkedList() {
    LinkedListNode* current = head;
    while (current != nullptr) {
        LinkedListNode* next = current->next;
        delete current;
        current = next;
    }
}
void LinkedList::removeOrders(Order* order) {
    LinkedListNode* current = head;
    LinkedListNode* prev = nullptr;

    while (current != nullptr && current->order != order) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        return; // Order not found
    }

    if (prev == nullptr) {
        head = current->next;
    }
    else {
        prev->next = current->next;
    }
    removeOrderFromFile(order);
    delete current;
}
void LinkedList::insertOrder(Order* order) {
    LinkedListNode* newNode = new LinkedListNode(order);  // Create a new node with the given order

    if (head == nullptr) {  
        head = newNode;  // If the list is empty, make the new node the head
    }
    else {  
        LinkedListNode* current = head;  // Start from the head of the list
        LinkedListNode* prev = nullptr;  // To keep track of the previous node

        // The condition current->order->price >= order->price ensures that the list is maintained in descending order of prices.
        while (current != nullptr && current->order->price >= order->price) {
            prev = current;  
            current = current->next;  
        }

        if (prev == nullptr) {  // If the new node should be inserted at the beginning
            newNode->next = head; 
            head = newNode;  
        }
        else {  // If the new node should be inserted in the middle or end
            prev->next = newNode;  // Link the previous node to the new node
            newNode->next = current;  // Link the new node to the next node in the list
        }
    }
}

void LinkedList::displayOrders() const {
    LinkedListNode* current = head;
    while (current != nullptr) {
        cout << "Order ID: " << current->order->id
            << ", Stock Symbol: " << current->order->stockSymbol
            << ", Quantity: " << current->order->quantity
            << ", Price: " << current->order->price
            << ", Type: " << (current->order->type == BUY ? "Buy" : "Sell")
            << ", Timestamp: " << formatTimestamp(current->order->timestamp)
            << endl;
        current = current->next;
    }
}  