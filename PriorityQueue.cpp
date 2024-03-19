#include "PriorityQueue.h"

// Constructor
PriorityQueue::PriorityQueue() {}

// Helper function for heap operations

void PriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent].second < heap[index].second) {
            swap(heap[parent], heap[index]);
            index = parent;
        } else {
            break;
        }
    }
}

void PriorityQueue::heapifyDown(int index) {
    int n = heap.size();
    while (true) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        if (leftChild < n && heap[leftChild].second > heap[largest].second)
            largest = leftChild;

        if (rightChild < n && heap[rightChild].second > heap[largest].second)
            largest = rightChild;

        if (largest != index) {
            swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

// Insert a string with its associated priority
void PriorityQueue::insert(const string& str, int priority) {
    heap.push_back(make_pair(str, priority));
    heapifyUp(heap.size() - 1);
}

// Retrieve and remove the string with the maximum priority

string PriorityQueue::extractMax() {
    if (heap.empty()) {
        throw std::logic_error("Priority queue is empty");
    }

    std::string maxStr = heap[0].first;
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
    return maxStr;
}

// Check if the priority queue is empty
bool PriorityQueue::empty() const {
    return heap.empty();
}
