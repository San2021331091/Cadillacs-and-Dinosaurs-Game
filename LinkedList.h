#ifndef CADILLACS_AND_DINOSAURS_GAME_LINKEDLIST_H
#define CADILLACS_AND_DINOSAURS_GAME_LINKEDLIST_H

#include "Game.h"


class Node {
public:
    string data;
    Node* next;

    explicit Node(string  imagePath) : data(std::move(imagePath)), next(nullptr) {}
};

class LinkedList {

public:
    static Node *head;
    LinkedList(){
        head = nullptr;
    }
    static int getSize();
    static void append(const string& imagePath);
    static string getFilePathAt(int index) ;


};

#endif



