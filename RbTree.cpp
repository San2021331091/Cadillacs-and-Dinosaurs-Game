#include "RbTree.h"



// Rotate left at given node
void red_black_tree::rotate_left(node*& root) {
    node* child = root->right;
    root->right = child->left;
    
    if (root->right != nullptr)
        root->right->parent = root;
    
    child->parent = root->parent;
    
    if (root->parent == nullptr)
        this->root = child;
    else if (root == root->parent->left)
        root->parent->left = child;
    else
        root->parent->right = child;
    
    child->left = root;
    root->parent = child;
}


// Rotate right at given node
void red_black_tree::rotate_right(node*& root) {
    node* child = root->left;
    root->left = child->right;
    
    if (root->left != nullptr)
        root->left->parent = root;
    
    child->parent = root->parent;
    
    if (root->parent == nullptr)
        this->root = child;
    else if (root == root->parent->left)
        root->parent->left = child;
    else
        root->parent->right = child;
    
    child->right = root;
    root->parent = child;
}


// Fix the Red-Black tree after standard BST insertion
void red_black_tree::fix_violation(node*& root) {
    node* parent = nullptr;
    node* grand_parent = nullptr;
    
    while (root != this->root && root->node_color != BLACK && root->parent->node_color == RED) {
        parent = root->parent;
        grand_parent = parent->parent;
        
        // Case A: Parent of root is left child of Grand-parent of root
        if (parent == grand_parent->left) {
            node* uncle = grand_parent->right;
            // Case 1: Uncle of root is also red
            if (uncle != nullptr && uncle->node_color == RED) {
                grand_parent->node_color = RED;
                parent->node_color = BLACK;
                uncle->node_color = BLACK;
                root = grand_parent;
            } else {
                // Case 2: Root is right child of its parent
                if (root == parent->right) {
                    rotate_left(parent);
                    root = parent;
                    parent = root->parent;
                }
                // Case 3: Root is left child of its parent
                rotate_right(grand_parent);
                swap(parent->node_color, grand_parent->node_color);
                root = parent;
            }
        } else { // Case B: Parent of root is right child of Grand-parent of root
            node* uncle = grand_parent->left;
            // Case 1: Uncle of root is also red
            if (uncle != nullptr && uncle->node_color == RED) {
                grand_parent->node_color = RED;
                parent->node_color = BLACK;
                uncle->node_color = BLACK;
                root = grand_parent;
            } else {
                // Case 2: Root is left child of its parent
                if (root == parent->left) {
                    rotate_right(parent);
                    root = parent;
                    parent = root->parent;
                }
                // Case 3: Root is right child of its parent
                rotate_left(grand_parent);
                swap(parent->node_color, grand_parent->node_color);
                root = parent;
            }
        }
    }
    this->root->node_color = BLACK;
}






// Insert a new node with given data
void red_black_tree::insert(const int data, const wchar_t* value) {
    node* new_node = new node(data, value);
    this->root = insert_bst(this->root, new_node);
    fix_violation(new_node);
}

// Insert a new node in BST manner
node* red_black_tree::insert_bst(node*& root, node*& new_node) {
    if (root == nullptr)
        return new_node;

    if (new_node->data < root->data) {
        root->left = insert_bst(root->left, new_node);
        root->left->parent = root;
    } else if (new_node->data > root->data) {
        root->right = insert_bst(root->right, new_node);
        root->right->parent = root;
    }
    return root;
}



// Search for a node with given data
node* red_black_tree::search(node*& root, int data) {
    if (root == nullptr || root->data == data)
        return root;

    if (root->data < data)
        return search(root->right, data);

    return search(root->left, data);
}



const wchar_t* red_black_tree::search(const int data) {
    node* result = search(this->root, data);
    return result != nullptr ? result->value : L"Value not found";
}
