#ifndef CADILLACS_AND_DINOSAURS_GAME_FENWICK_H
#define CADILLACS_AND_DINOSAURS_GAME_FENWICK_H

#include "Game.h"


class FenwickTree {
private:
    vector<float> bit;
    map<float, float> indexMap; // Map floating-point indices to their corresponding discrete indices
    float& minVal; // Reference to min value
    float& maxVal; // Reference to max value

public:
    FenwickTree(int n, float& minValRef, float& maxValRef);
    void update(float index, float val);
    float query(float index);
    float range_sum(float start, float end);
    void addIndex(float index);
};

#endif 
