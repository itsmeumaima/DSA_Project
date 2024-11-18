#ifndef STACK_H
#define STACK_H

template<typename T>
class Stack {
private:
    T* arr;       // Pointer to array of elements
    int capacity; // Maximum capacity of the stack
    int top;      // Index of the top element

    // Helper function to resize the stack
    void resize();

public:
    // Constructor
    Stack();

    // Destructor
    ~Stack();

    // Push an element to the stack
    void push(const T& value);

    // Pop an element from the stack
    T pop();

    // Check if the stack is empty
    bool isEmpty() const;

    // Check if the stack is full
    bool isFull() const;
};

#include "Stack.tpp" // Include the template implementation file

#endif // STACK_H
