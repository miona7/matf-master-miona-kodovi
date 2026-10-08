#include <iostream>
#include <vector>

typedef struct Node {
    int key;
    int height;
    int size; // broj cvorova u podstablu
    struct Node* left;
    struct Node* right;
} Node;

// pomocne funkcije
int height(Node* n) {
    return n != nullptr ? n->height : 0;
}

int size(Node* n) {
    return n != nullptr ? n->size : 0;
}

int max(int a, int b) {
    return (a >= b) ? a : b;
}

// kreiranje novog cvora
Node* newNode(int key) {
    Node* n = (Node*) malloc(sizeof(Node));
    n->key = key;
    n->height = 1;
    n->size = 1;
    n->left = n->right = nullptr;
    return n;
}

// azuriranje visine i velicine cvora na osnovu dece
void update(Node* n) {
    if(n == nullptr) {
        return;
    }
    
    n->height = 1 + max(height(n->left), height(n->right));
    n->size = 1 + size(n->left) + size(n->right);
}

// balans faktor
int getBalance(Node* n) {
    if(n == nullptr) {
        return 0;
    }
    
    return height(n->left) - height(n->right);
}

Node* rightRotation(Node* x) {
    Node* y = x->left;
    Node* tmp = y->right;

    // rotacija
    y->right = x;
    x->left = tmp;

    update(x);
    update(y);

    // novi koren
    return y; 
}

Node* leftRotation(Node* x) {
    Node* y = x->right;
    Node* tmp = y->left;

    // rotacija
    y->left = x;
    x->right = tmp;

    update(x);
    update(y);

    // novi koren
    return y; 
}

Node* insert(Node* root, int key) {
    if(root == nullptr) {
        return newNode(key);
    }

    if(key < root->key) {
        root->left = insert(root->left, key);
    } else if(key > root->key) {
        root->right = insert(root->right, key);
    } else {
        // duplikate ignorisemo
        return root;
    }

    update(root);
    
    int balance = getBalance(root);

    // left left
    if(balance > 1 && key < root->left->key) {
        return rightRotation(root);
    }

    // right right
    if(balance < -1 && key > root->right->key) {
        return leftRotation(root);
    }

    // left right
    if(balance > 1 && key > root->left->key) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

    // right left
    if(balance < -1 && key < root->right->key) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }
    
    return root;
}

// sortiran ispis
void inorderPrint(Node* root) {
    if(root == nullptr) {
        return;
    }
    
    inorderPrint(root->left);
    std::cout << "key = " << root->key << " (size = " << root->size << ")\n"; 
    inorderPrint(root->right);
}

int main() {
    Node* root = nullptr;

    std::vector<int> values = {20, 10, 30, 5, 15, 25, 35};
   
    for(int i = 0; i < values.size(); i++) {
        root = insert(root, values[i]);
    }

    std::cout << "AVL stablo:" << std::endl;
    inorderPrint(root);
    
    return 0;
}