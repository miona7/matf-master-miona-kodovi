/* Održavanje medijane u dinamičkom toku podataka

Konstruisati strukturu podataka koja podržava sledeće operacije u realnom vremenu:
- umetni(x) – Dodavanje elementa x u kolekciju
- obrisi(x) – Brisanje elementa x iz kolekcije
- nadji_medijanu() – Pronalaženje medijane trenutnih elemenata

Kako koristi AVL stablo: Prošireno AVL stablo (gde svaki čvor zna veličinu svog podstabla) 
omogućava pronalazak k-tog najmanjeg elementa u vremenu O(log n). Pošto je medijana zapravo
(n/2)-ti po redu element, operacija find_median() traje O(log n) (ili O(1) ako se čuva pokazivač 
na medijana-čvor koji se pomera pri ubacivanju/brisanju).
Složenost: Svaka operacija u O(log n) */

#include <iostream>
#include <string>
#include <algorithm>

struct Cvor {
    int kljuc;
    Cvor* levo;
    Cvor* desno;
    int visina;
    int faktorRavnoteze;
    bool oznaka = false;
    int velicina; // velicina stabla

    void azurirajVrednosti() {
        int visinaLevo = levo != nullptr ? levo->visina : -1;
        int visinaDesno = desno != nullptr ? desno->visina : -1;
        visina = 1 + std::max(visinaLevo, visinaDesno);
        faktorRavnoteze = visinaLevo - visinaDesno;

        int velicinaLevo = levo != nullptr ? levo->velicina : 0;
        int velicinaDesno = desno != nullptr ? desno->velicina: 0;
        velicina = 1 + velicinaLevo + velicinaDesno;
    }
};

inline Cvor* rotacijaLL(Cvor* A) {
    Cvor* B = A->levo;
    A->levo = B->desno;
    B->desno = A;
    A->azurirajVrednosti();  
    B->azurirajVrednosti();
    return B;
}

inline Cvor* rotacijaLR(Cvor* A) {
    Cvor* B = A->levo;
    Cvor* D = B->desno;
    B->desno = D->levo;
    A->levo = D->desno;
    D->levo = B;
    D->desno = A;
    A->azurirajVrednosti();
    B->azurirajVrednosti();
    D->azurirajVrednosti();
    return D;
}

inline Cvor* rotacijaRL(Cvor* A) {
    Cvor* B = A->desno;
    Cvor* D = B->levo;
    B->levo = D->desno;
    A->desno = D->levo;
    D->desno = B;
    D->levo = A;
    A->azurirajVrednosti();
    B->azurirajVrednosti();
    D->azurirajVrednosti();
    return D;
}

inline Cvor* rotacijaRR(Cvor* A) {
    Cvor* B = A->desno;
    A->desno = B->levo;
    B->levo = A;
    A->azurirajVrednosti();
    B->azurirajVrednosti();
    return B;
}

inline Cvor* balansiraj(Cvor* koren) {
    if(koren == nullptr) {
        return nullptr;
    }
    
    koren->azurirajVrednosti();
    
    if(koren->faktorRavnoteze == 2 && koren->levo->faktorRavnoteze >= 0) {
        return rotacijaLL(koren);
    }

    if(koren->faktorRavnoteze == 2 && koren->levo->faktorRavnoteze < 0) {
        return rotacijaLR(koren);
    }

    if(koren->faktorRavnoteze == -2 && koren->desno->faktorRavnoteze > 0) {
        return rotacijaRL(koren);
    }
    
    if(koren->faktorRavnoteze == -2 && koren->desno->faktorRavnoteze <= 0) {
        return rotacijaRR(koren);
    }
    
    return koren;
}

inline bool pronadji(Cvor* koren, int x) {
    if(koren == nullptr) {
        return false;
    }

    if(x < koren->kljuc) {
        return pronadji(koren->levo, x);
    }

    if(x > koren->kljuc) {
        return pronadji(koren->desno, x);
    }

    return true;
}

inline Cvor* umetni(Cvor* koren, int x) {
    if(koren == nullptr) {
        return new Cvor{.kljuc = x, .levo = nullptr, .desno = nullptr,
                        .visina = 0, .faktorRavnoteze = 0, .velicina = 1};
    }

    if(x < koren->kljuc) {
        koren->levo = umetni(koren->levo, x);
    } else if(x > koren->kljuc) {
        koren->desno = umetni(koren->desno, x);
    }

    return balansiraj(koren);
}

inline Cvor* najmanji(Cvor* koren) {
    if(koren->levo == nullptr) {
        return koren;
    }

    return najmanji(koren->levo);
}

inline Cvor* brisi(Cvor* koren, int x) {
    if(koren == nullptr) {
        return nullptr;
    }

    if(x < koren->kljuc) {
        koren->levo = brisi(koren->levo, x);
        return balansiraj(koren);
    }

    if(x > koren->kljuc) {
        koren->desno = brisi(koren->desno, x);
        return balansiraj(koren);
    }

    if(koren->levo == nullptr) {
        Cvor* r = koren->desno;
        delete koren;
        return r;
    }

    if(koren->desno == nullptr) {
        Cvor* l = koren->levo;
        delete koren;
        return l;
    }

    Cvor* sledeci = najmanji(koren->desno);
    koren->kljuc = sledeci->kljuc;
    koren->desno = brisi(koren->desno, sledeci->kljuc);
    
    return balansiraj(koren);
}

inline void ispis(Cvor* koren, int dubina) {
    if(koren == nullptr) {
        return;
    }

    ispis(koren->desno, dubina + 1);
    std::cout << std::string(4*dubina, ' ');
    std::cout << koren->kljuc << '\n';
    ispis(koren->levo, dubina + 1);
}

inline void oslobodi(Cvor* koren) {
    if(koren == nullptr) {
        return;
    }

    oslobodi(koren->levo);
    oslobodi(koren->desno);
    delete koren;
}

inline void kTiElement(Cvor* koren, int k, int& rez) {
    if(koren == nullptr || k < 0 || k >= koren->velicina) {
        return;
    }

    int velicinaLevo = koren->levo != nullptr ? koren->levo->velicina : 0;

    if(k < velicinaLevo) {
        kTiElement(koren->levo, k, rez);
    } else if(velicinaLevo == k) {
        rez = koren->kljuc;
        return;
    } else {
        kTiElement(koren->desno, k - velicinaLevo - 1, rez);
    }
}

inline bool nadji_medijanu(Cvor* koren, double& medijana) {
    if(koren == nullptr) {
        return false;
    }

    int n = koren->velicina; // velicina celog stabla
    if(n % 2 == 0) {
        int a; int b;
        kTiElement(koren, n / 2, a);
        kTiElement(koren, n / 2 - 1, b);
        medijana = (a + b) / 2.0;
    } else {
        int a;
        kTiElement(koren, n / 2, a);
        medijana = a;
    }

    return true;
}

int main() {
    Cvor* stablo = nullptr;
    stablo = umetni(stablo, 40);
    stablo = umetni(stablo, 60);
    stablo = umetni(stablo, 20);
    stablo = umetni(stablo, 30);
    stablo = umetni(stablo, 10);
    stablo = umetni(stablo, 70);
    stablo = umetni(stablo, 50);
    // stablo = umetni(stablo, 80);

    double medijana;
    if(nadji_medijanu(stablo, medijana)) {
        std::cout << medijana << std::endl;
    }

    oslobodi(stablo);
}