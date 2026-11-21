#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cassert>

struct Node {
    int key;
    bool oznaka = false;
    Node* levo = nullptr;
    Node* desno = nullptr;
};

// Dopuniti samo delove [A]-[E].
// nullptr: vraca 0.
// Neprazno podstablo: apsolutna vrednost je njegova visina.
// Negativan rezultat: podstablo sadrzi bar jedan S-AVL cvor.
// Pozitivan rezultat: podstablo ne sadrzi S-AVL cvorove.
int oznaciSAVL(Node* x) {
    // [A] (2 poena) Prazno stablo i resetovanje oznake.
    if(x == nullptr) {
        return 0;
    }
    x->oznaka = false;

    // [B] (2 poena) Rekurzivna obrada levog i desnog podstabla.
    int l = oznaciSAVL(x->levo);
    int d = oznaciSAVL(x->desno);

    if(l > 0 && d > 0) {
        // [C] (3 poena) Provera visina, oznacavanje i rezultat.
        if(l == d) {
            x->oznaka = true;
            return -(l + 1);
        }
        return std::max(l, d) + 1;
    } else if(l < 0 || d < 0) {
        // [D] (2 poena) U bar jednom podstablu postoji S-AVL cvor.
        return -(std::max(std::abs(l), std::abs(d)) + 1);
    } else {
        // [E] (1 poen) Preostali slucaj.
        return std::max(l, d) + 1;
    }
}

#ifndef SAVL_BEZ_MAIN
#include <iostream>

// Ispisuje oznacene cvorove inorder obilaskom.
void ispisiOznacene(const Node* x) {
    if(x == nullptr) {
        return;
    }

    ispisiOznacene(x->levo);
    
    if(x->oznaka) {
        std::cout << x->key << ' ';
    }
    
    ispisiOznacene(x->desno);
}

// Pomocna funkcija koja skuplja oznacene kljuceve u vektor (inorder)
void skupiOznacene(const Node* x, std::vector<int>& rez) {
    if(x == nullptr) {
        return;
    }

    skupiOznacene(x->levo, rez);
    
    if(x->oznaka) {
        rez.push_back(x->key);
    }
    
    skupiOznacene(x->desno, rez);
}

int main() {
    Node n40{40}, n20{20}, n60{60};
    Node n10{10}, n30{30}, n50{50}, n70{70};
    n40.levo = &n20;
    n40.desno = &n60;
    n20.levo = &n10;
    n20.desno = &n30;
    n60.levo = &n50;
    n60.desno = &n70;

    const int rezultat = oznaciSAVL(&n40);
    std::cout << "Oznaceni cvorovi: ";
    ispisiOznacene(&n40);
    std::cout << "\nPovratna vrednost: " << rezultat << '\n';
    // Ocekuje se: oznaceni cvorovi 20 i 60, rezultat -3.

    // TEST 1: Prazno stablo [A]
    {
        assert(oznaciSAVL(nullptr) == 0);
    }

    // TEST 2: Jedan cvor (list) -> l=0, d=0 -> ulazi u [E], vraca 1, nije oznacen
    {
        Node n1{10, true}; // namerno stavljamo true da testiramo reset u [A]
        int visina = oznaciSAVL(&n1);
        std::vector<int> oz;
        skupiOznacene(&n1, oz);

        assert(visina == 1);
        assert(oz.empty());
    }

    // TEST 3: Cvor sa samo jednim detetom -> l=1, d=0 -> ulazi u [E], vraca 2
    {
        Node n10{10}, n20{20};
        n20.levo = &n10;

        int visina = oznaciSAVL(&n20);
        std::vector<int> oz;
        skupiOznacene(&n20, oz);

        assert(visina == 2);
        assert(oz.empty());
    }

    // TEST 4: Oba podstabla neprazna, ali razlicite visine -> [C] gde l != d
    //       30
    //      /  \
    //     20   40
    //    /
    //   10
    // Visina levo = 2, desno = 1 -> nisu jednake -> 30 NIJE S-AVL, vraca 3
    {
        Node n10{10}, n20{20}, n30{30, true}, n40{40};
        n30.levo = &n20;
        n30.desno = &n40;
        n20.levo = &n10;

        int visina = oznaciSAVL(&n30);
        std::vector<int> oz;
        skupiOznacene(&n30, oz);

        assert(visina == 3);
        assert(oz.empty());
    }

    // TEST 5: Tvoj primer iz main-a (savrseno stablo visine 3)
    // 20 i 60 su S-AVL (visina 2), 40 nije jer vec ima S-AVL potomke -> vraca -3
    {
        Node n40{40, true}, n20{20}, n60{60};
        Node n10{10}, n30{30}, n50{50}, n70{70};
        n40.levo = &n20; n40.desno = &n60;
        n20.levo = &n10; n20.desno = &n30;
        n60.levo = &n50; n60.desno = &n70;

        int visina = oznaciSAVL(&n40);
        std::vector<int> oz;
        skupiOznacene(&n40, oz);

        assert(visina == -3);
        assert((oz == std::vector<int>{20, 60}));
    }

    // TEST 6: Jedno podstablo ima S-AVL (negativna visina), drugo je dublje ali nema S-AVL
    // Testira [D]: -(max(|l|, |d|) + 1)
    //         50
    //        /  \
    //      20    70
    //     / \      \
    //   10  30      80
    //                 \
    //                  90
    // Levo od 50: l = -2 (20 je S-AVL)
    // Desno od 50: d = 3 (lanac 70-80-90, nema S-AVL)
    // Koren 50 treba da vrati -(max(2, 3) + 1) = -4, oznacen samo 20.
    {
        Node n50{50}, n20{20}, n10{10}, n30{30}, n70{70}, n80{80}, n90{90};
        n50.levo = &n20;
        n20.levo = &n10;
        n20.desno = &n30;

        n50.desno = &n70;
        n70.desno = &n80;
        n80.desno = &n90;

        int visina = oznaciSAVL(&n50);
        std::vector<int> oz;
        skupiOznacene(&n50, oz);

        assert(visina == -4);
        assert((oz == std::vector<int>{20}));
    }

    std::cout << "Svi test primeri su uspesno prosli!\n";
    return 0;
}
#endif
