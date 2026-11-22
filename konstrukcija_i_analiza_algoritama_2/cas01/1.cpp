/* Brojanje manjih elemenata sa desne strane

Problem: Dat je niz celih brojeva A dužine n. Za svaki element A[i] treba odrediti koliko 
ima elemenata desno od njega (A[j] gde je j > i) koji su strogo manji od A[i]. */

#include <iostream>
#include <vector>
#include <cassert>

typedef struct Node {
    int key;
    int height; 
    int size;
    int count; // broj duplikata za vrednost key
    struct Node* left;
    struct Node* right;
} Node;

int height(Node* n) {
    return n != nullptr ? n->height : 0;
}

int size(Node* n) {
    return n != nullptr ? n->size : 0;
}

int max(int a, int b) {
    return (a >= b) ? a : b;
}

Node* newNode(int key) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->height = 1;
    n->size = 1;
    n->count = 1;
    n->left = n->right = nullptr;
    return n;
}

void update(Node* n) {
    if(n == nullptr) {
        return;
    }
    
    n->height = 1 + max(height(n->left), height(n->right));
    n->size = n->count + size(n->left) + size(n->right);
}

int getBalance(Node* n) {
    if(n == nullptr) {
        return 0;
    }
    
    return height(n->left) - height(n->right);
}

Node* rightRotation(Node* x) {
    Node* y = x->left;
    Node* tmp = y->right;

    y->right = x;
    x->left = tmp;

    update(x);
    update(y);

    return y; 
}

Node* leftRotation(Node* x) {
    Node* y = x->right;
    Node* tmp = y->left;

    y->left = x;
    x->right = tmp;

    update(x);
    update(y);

    return y; 
}

Node* insert(Node* root, int key, int& smallerCount) {
    if(root == nullptr) {
        return newNode(key);
    }

    if(key < root->key) { // kada idemo levo, nema manjih elemenata
        root->left = insert(root->left, key, smallerCount);
    } else if(key > root->key) {
        // kada idemo desno, svi elementi iz levog podstabla su manji, kao i element iz trenutnog cvora
        smallerCount += size(root->left) + root->count;
        root->right = insert(root->right, key, smallerCount);
    } else { // inace manji elementi su u levom podstablu
        root->count++;
        smallerCount += size(root->left);
        update(root);
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

std::vector<int> countSmaller(const std::vector<int>& nums) {
    Node* root = nullptr;
    std::vector<int> res(nums.size(), 0);
    
    // elemente niza ubacujemo u stablo s desna ulevo, da bi u trenutku kada se u stablo
    // dodaje element na i-toj poziciji, u stablu vec bili svi elementi koji su sa njegove
    // desne strane
    for(int i = nums.size() - 1; i >= 0; i--) {
        root = insert(root, nums[i], res[i]);
    }

    return res;
}

int main() {
    // osnovni primer iz zadatka
    assert((countSmaller({34, 54, 1, 2, 9, 0, -2, -5, 3, 30}) ==
            std::vector<int>{8, 8, 3, 3, 4, 2, 1, 0, 0, 0}));

    // strogo rastući niz (nema manjih desno)
    assert((countSmaller({1, 2, 3, 4, 5}) ==
            std::vector<int>{0, 0, 0, 0, 0}));

    // strogo opadajući niz (svi desno su manji)
    assert((countSmaller({5, 4, 3, 2, 1}) ==
            std::vector<int>{4, 3, 2, 1, 0}));

    // svi elementi isti (traže se strogo manji)
    assert((countSmaller({7, 7, 7, 7}) ==
            std::vector<int>{0, 0, 0, 0}));

    // kombinacija duplikata i različitih brojeva
    assert((countSmaller({5, 2, 6, 2, 1}) ==
            std::vector<int>{3, 1, 2, 1, 0}));

    // negativni brojevi i nula
    assert((countSmaller({-1, -5, 0, -2, 3}) ==
            std::vector<int>{2, 0, 1, 0, 0}));

    // jedan element i prazan niz
    assert((countSmaller({42}) == std::vector<int>{0}));
    assert(countSmaller({}).empty());

    std::cout << "Svi testovi su uspesno prosli!\n";
    return 0;
}