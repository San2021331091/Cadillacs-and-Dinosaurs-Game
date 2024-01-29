#ifndef CADILLACS_AND_DINOSAURS_GAME_STACK_H
#define CADILLACS_AND_DINOSAURS_GAME_STACK_H


template <typename T>
class Node {
public:
    T data;
    Node<T>* next;

    explicit Node(T val);
};

template <typename T>
class Stack {
private:
    Node<T>* top;

public:
    Stack();
    ~Stack(); // Destructor to free allocated memory

    void push(T val);
    void pop();
    [[nodiscard]] bool isEmpty() const;
    T peek() const;
};

#endif // STACK_H

