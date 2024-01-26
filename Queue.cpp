#include "Queue.h"
#include "Game.h"

template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr) {}

template <typename T>
Queue<T>::~Queue() {
    clear();
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return front == nullptr;
}


template< typename T>
void Queue<T>::enqueue(const T& item) {
    auto* newNode = new Node<T>(item);

    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}
template <typename T>
void Queue<T>::dequeue() {
    if (isEmpty()) {
        cerr << "Queue is empty. Cannot dequeue." << std::endl;
        return;
    }

    Node<T>* temp = front;
    front = front->next;
    delete temp;

    if (front == nullptr) {
        rear = nullptr;
    }
}


template <typename T>
void Queue<T>::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}

template <typename T>
T Queue<T>::getFront() const {
    if (isEmpty()) {
        // Handle the case when the queue is empty
        throw runtime_error("Queue is empty");
    }
    return front->data;
}
// Explicit instantiation for commonly used type
template class Queue<const wchar_t *>;

