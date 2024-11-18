#ifndef STACK_TPP
#define STACK_TPP

#include <stdexcept>
const int DEFAULT_CAPACITY = 10;

template<typename T>
Stack<T>::Stack() : capacity(DEFAULT_CAPACITY), top(-1) {
    arr = new T[capacity];
}

template<typename T>
Stack<T>::~Stack() {
    delete[] arr;
}

template<typename T>
void Stack<T>::resize() {
    capacity *= 2;
    T* newArr = new T[capacity];
    for (int i = 0; i <= top; ++i) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
}

template<typename T>
void Stack<T>::push(const T& value) {
    if (isFull()) {
        resize();
    }
    arr[++top] = value;
}

template<typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty");
    }
    return arr[top--];
}

template<typename T>
bool Stack<T>::isEmpty() const {
    return top == -1;
}

template<typename T>
bool Stack<T>::isFull() const {
    return top == capacity - 1;
}

#endif // STACK_TPP
