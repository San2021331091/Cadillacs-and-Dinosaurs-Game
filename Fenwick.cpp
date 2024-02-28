#include "Fenwick.h"



FenwickTree::FenwickTree(int n, float& minValRef, float& maxValRef) : bit(n + 1, 0), minVal(minValRef), maxVal(maxValRef) {}

void FenwickTree::update(float index, float val) {
    if (val < minVal)
        minVal = val; // Update minVal only if val is less than current minVal
    if (val > maxVal)
        maxVal = val; // Update maxVal only if val is greater than current maxVal
    int i = indexMap[index];
    i++; // Convert to 1-based indexing
    while (i < bit.size()) {
        bit[i] += val;
        i += i & -i; // Move to the parent node
    }
}

float FenwickTree::query(float index) {
    int i = indexMap[index];
    i++; // Convert to 1-based indexing
    float sum = 0;
    while (i > 0) {
        sum += bit[i];
        i -= i & -i; // Move to the parent node
    }
    return sum;
}

float FenwickTree::range_sum(float start, float end) {
    return query(end) - query(start - 0.01f); // Adjust for floating-point precision
}

void FenwickTree::addIndex(float index) {
    if (indexMap.find(index) == indexMap.end()) {
        indexMap[index] = indexMap.size(); // Assign the next available discrete index
    }
}
