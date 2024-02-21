#include "HashTable.h"

template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
    }
}

template<typename KeyType, typename ValueType>
int HashTable<KeyType, ValueType>::hash(const KeyType& key) {
    std::hash<KeyType> hash_fn;
    return hash_fn(key) % TABLE_SIZE;
}

template<typename KeyType, typename ValueType>
ValueType& HashTable<KeyType, ValueType>::operator[](const KeyType& key) {
    int index = hash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    ValueType default_value{};
    insert(key, default_value);
    return table[index]->value;
}

template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    int index = hash(key);
    HashNode* newNode = new HashNode(key, value);

    if (table[index] == nullptr) {
        table[index] = newNode;
    } else {
        HashNode* current = table[index];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::search(const KeyType& key) {
    int index = hash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template class HashTable<string, bool>;
template class HashTable<string, vector<string>>;
