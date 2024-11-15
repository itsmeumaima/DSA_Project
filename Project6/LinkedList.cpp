#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio> 
#include <ctime>

#include <cstdio>  // For std::remove and std::rename

void removeOrderFromFile(Order* order) {
    std::ifstream infile("Data1.txt");
    std::ofstream tempfile("temp.txt");

    if (infile.is_open() && tempfile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::stringstream ss(line);
            int id;
            std::string symbol, orderType, timeInForce;
            double price;
            int quantity;

            ss >> id; ss.ignore();
            std::getline(ss, symbol, ',');
            ss >> price; ss.ignore();
            ss >> quantity; ss.ignore();
            std::getline(ss, orderType, ',');
            std::getline(ss, timeInForce);

            // If the order ID matches the one to remove, skip writing it to the file
            if (id != order->getId()) {
                tempfile << line << std::endl;
            }
        }

        infile.close();
        tempfile.close();

        // Replace the original file with the updated file
        if (std::remove("Data1.txt") != 0) {
            std::cerr << "Error deleting the original file.\n";
        }

        if (std::rename("temp.txt", "Data1.txt") != 0) {
            std::cerr << "Error renaming the temporary file.\n";
        }
    }
    else {
        std::cerr << "Unable to open the file for updating.\n";
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
    LinkedListNode* newNode = new LinkedListNode(order);
    if (head == nullptr) {
        head = newNode;
    }
    else {
        LinkedListNode* current = head;
        LinkedListNode* prev = nullptr;

        while (current != nullptr && current->order->price >= order->price) {
            prev = current;
            current = current->next;
        }

        if (prev == nullptr) {
            newNode->next = head;
            head = newNode;
        }
        else {
            prev->next = newNode;
            newNode->next = current;
        }
    }
}
void LinkedList::clear() {
    LinkedListNode* current = head;
    while (current != nullptr) {
        LinkedListNode* next = current->next;
        delete current;  // Delete the node
        current = next;
    }
    head = nullptr;  // Reset the head to nullptr to indicate an empty list
}

void LinkedList::displayOrders() const {
    LinkedListNode* current = head;
    while (current != nullptr) {
        std::cout << "Order ID: " << current->order->id
            << ", Stock Symbol: " << current->order->stockSymbol
            << ", Quantity: " << current->order->quantity
            << ", Price: " << current->order->price
            << ", Type: " << (current->order->type == BUY ? "Buy" : "Sell")
            << ", TIF: " << current->order->tif
            << ", Timestamp: " << formatTimestamp(current->order->timestamp)
            << std::endl;
        current = current->next;
    }
}
