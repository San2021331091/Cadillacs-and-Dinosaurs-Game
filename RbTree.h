#ifndef CADILLACS_AND_DINOSAURS_GAME_RbTree_H
#define CADILLACS_AND_DINOSAURS_GAME_RbTree_H



#include"Game.h"



enum color { RED, BLACK };

class node {
public:
    int data;
    const wchar_t* value;
    color node_color; 
    node *left, *right, *parent;

    node(int data, const wchar_t* value) : data(data), value(value), node_color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class red_black_tree {
private:
    node* root;

    void rotate_left(node*&);
    void rotate_right(node*&);
    void fix_violation(node*&);
    node* insert_bst(node*&, node*&);
    node* search(node*&, int);

public:
    red_black_tree() : root(nullptr) {}

    void insert(const int data, const wchar_t* value);
    const wchar_t* search(const int data);
};

#endif 