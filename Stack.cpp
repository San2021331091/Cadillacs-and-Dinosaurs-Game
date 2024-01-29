#include "Stack.h"
#include "Game.h"

// Node class implementation
template <typename T>
Node<T>::Node(T val) : data(val), next(nullptr) {}

// Stack class implementation
template <typename T>
Stack<T>::Stack() : top(nullptr) {}

template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

template <typename T>
void Stack<T>::push(T val) {
    auto* newNode = new Node<T>(val);
    newNode->next = top;
    top = newNode;
}

template <typename T>
void Stack<T>::pop() {
    if (isEmpty()) {
        cout << "Stack underflow. Cannot pop from an empty stack." << endl;
        return;
    }

    Node<T>* temp = top;
    top = top->next;
    delete temp;
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return top == nullptr;
}

template <typename T>
T Stack<T>::peek() const {
    if (isEmpty()) {
        cerr << "Error: Cannot peek from an empty stack." << endl;
        return T();
    }
    return top->data;
}

template class Stack<string>;