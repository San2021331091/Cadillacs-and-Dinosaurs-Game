#ifndef CADILLACS_AND_DINOSAURS_GAME_PRIORITYQUEUE_H
#define CADILLACS_AND_DINOSAURS_GAME_PRIORITYQUEUE_H

#include  "Game.h"

class PriorityQueue {
private:
    vector<pair<string, int>> heap;

    // Helper functions for heap operations
    void heapifyUp(int);
    void heapifyDown(int);

public:
    // Constructor
    PriorityQueue();

    // Insert a string with its associated priority
    void insert(const string& , int );

    // Retrieve and remove the string with the maximum priority
    string extractMax();

    // Check if the priority queue is empty
    bool empty() const;
};

#endif // PRIORITYQUEUE_H
