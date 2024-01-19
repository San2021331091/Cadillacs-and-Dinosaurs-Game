#include "LinkedList.h"

Node* LinkedList::head = nullptr;

void LinkedList::append(const string& imagePath) {
    Node* newNode = new Node(imagePath);

    if (head == nullptr) {
        // If the list is empty, make the new node the head
        head = newNode;
    } else {
        // If the list is not empty, traverse to the end and append the new node
        Node* p = head;
        while (p->next != nullptr)
            p = p->next;

        p->next = newNode;
        newNode->next = nullptr;
    }
}


int LinkedList::getSize() {
    Node* current = head;
    int size = 0;

    while (current != nullptr) {
        size++;
        current = current->next;
    }

    return size;
}

string LinkedList::getFilePathAt(int index) {
    Node* current = head;
    int currentIndex = 0;

    while (current != nullptr && currentIndex < index) {
        current = current->next;
        currentIndex++;
    }

    if (current == nullptr) {
        return "";
    }

    return current->data;
}

