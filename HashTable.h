#ifndef CADILLACS_AND_DINOSAURS_GAME_HASHTABLE_H
#define CADILLACS_AND_DINOSAURS_GAME_HASHTABLE_H

#include "Game.h"

template<typename KeyType, typename ValueType>
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    struct HashNode {
        KeyType key;
        ValueType value;
        HashNode* next;

        HashNode(const KeyType& key, const ValueType& value) : key(key), value(value), next(nullptr) {}
    };

    HashNode* table[TABLE_SIZE];

    int hash(const KeyType& key);

public:
    HashTable();
    ~HashTable();

    ValueType& operator[](const KeyType& key);
    void insert(const KeyType& key, const ValueType& value);
    bool search(const KeyType& key);
};


#endif
