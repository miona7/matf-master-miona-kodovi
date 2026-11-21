#include <iostream>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;

    Node(int k, Color c)
        : key(k), color(c), left(nullptr), right(nullptr) {}
};

static bool validateRB(Node* u, // vraca true ili false u zavisnosti da li je stablo validno ili ne
                       int blackCount, // koliko smo do sada videli crnih cvorova na putu od korena do ovog naseg cvora u
                       int& targetBlack) { // referentna (očekivana) crna visina koju moraju imati sve putanje 
    // --- 1) BAZA REKURZIJE ---
    // Ako smo došli do NIL lista (nullptr),
    // završena je jedna putanja.
    if(u == nullptr) {
	// DOPUNA
        // Ako još nemamo referentnu crnu visinu, postavljamo je sada.
        if(targetBlack == -1) {
            targetBlack = blackCount;
        }

        // Sva naredna NIL mesta moraju imati isti broj crnih čvorova.
        return blackCount == targetBlack;
    }

    // --- 2) AKO JE ČVOR CRN ---
    // Uvećavamo broj crnih čvorova na putanji.
    // DOPUNA
    if(u->color == BLACK) {
        blackCount++;
    }

    // --- 3) PROVERA CRVENOG ČVORA ---
    // Crveni čvor ne sme imati crveno dete.
    // DOPUNA
    if(u->color == RED) {
        if((u->left != nullptr && u->left->color == RED) || (u->right != nullptr && u->right->color == RED)) {
            return false;
        } 
    }

    // --- 4) REKURZIJA ---
    // Proveravamo levo i desno podstablo.
    // Oba moraju biti validna.
    // DOPUNA
    return validateRB(u->left, blackCount, targetBlack) && validateRB(u->right, blackCount, targetBlack);
}

bool isValidRBTree(Node* root) {
    if(root == nullptr) {
        return true;
    }

    // koren mora biti crn
    if(root->color != BLACK) {
        return false;
    }

    int targetBlack = -1;
    return validateRB(root, 0, targetBlack);
}

/* --------------------------------------------------
   POMOCNA FUNKCIJA ZA ISPIS
-------------------------------------------------- */

void printResult(Node* root) {
    if(isValidRBTree(root)) {
        cout << "Stablo je VALIDNO RB stablo.\n";
    } else {
        cout << "Stablo NIJE validno RB stablo.\n";
    }
}

/* --------------------------------------------------
   TEST PRIMERI
-------------------------------------------------- */

int main() {
    // ===== TEST 1: Ispravno RB stablo =====
    Node* root1 = new Node(10, BLACK);
    root1->left  = new Node(5, RED);
    root1->right = new Node(20, RED);
    root1->left->left   = new Node(3, BLACK);
    root1->left->right  = new Node(7, BLACK);
    root1->right->left  = new Node(15, BLACK);
    root1->right->right = new Node(25, BLACK);

    cout << "Test 1:\n";
    printResult(root1);


    // ===== TEST 2: Krsi pravilo (dva crvena uzastopno) =====
    Node* root2 = new Node(10, BLACK);
    root2->left = new Node(5, RED);
    root2->left->left = new Node(3, RED);  // greska

    cout << "\nTest 2:\n";
    printResult(root2);


    // ===== TEST 3: Krsi black-height pravilo =====
    Node* root3 = new Node(10, BLACK);
    root3->left = new Node(5, BLACK);
    root3->right = new Node(20, BLACK);
    root3->left->left = new Node(3, BLACK); // levi put ima vise crnih

    cout << "\nTest 3:\n";
    printResult(root3);

    return 0;
}
