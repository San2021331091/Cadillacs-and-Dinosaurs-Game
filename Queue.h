#ifndef CADILLACS_AND_DINOSAURS_GAME_QUEUE_H
#define CADILLACS_AND_DINOSAURS_GAME_QUEUE_H

#pragma once


template <typename T>
class Node {
public:
    T data;
    Node* next;

    explicit Node(T item) : data(item), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;

public:
    Queue();
    ~Queue();

    [[nodiscard]] bool isEmpty() const;
    void enqueue(const T& item);
    void dequeue();
    void clear();
    T getFront() const;
};



#endif
