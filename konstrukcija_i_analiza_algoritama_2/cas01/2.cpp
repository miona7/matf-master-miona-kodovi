// Napisati algoritam koji označava sve čvorove koji su S-AVL čvorovi, ali čiji nijedan pravi potomak nije S-AVL čvor. 
// Vreme izvršavanja treba da bude O(n).

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

    void azurirajVrednosti() {
        int visinaLevo = levo ? levo->visina : -1;
        int visinaDesno = desno ? desno->visina : -1;
        visina = 1 + std::max(visinaLevo, visinaDesno);
        faktorRavnoteze = visinaLevo - visinaDesno;
    }
};

inline Cvor* rotacijaLL(Cvor* A) {
    Cvor* B = A->levo;
    A->levo = B->desno;
    B->desno = A;
    A->azurirajVrednosti();  // A je sada ispod B.
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

inline Cvor *rotacijaRR(Cvor* A) {
    Cvor* B = A->desno;
    A->desno = B->levo;
    B->levo = A;
    A->azurirajVrednosti();
    B->azurirajVrednosti();
    return B;
}

inline Cvor* balansiraj(Cvor* koren) {
    if (koren == nullptr) {
        return nullptr;
    }
    
    koren->azurirajVrednosti();
    
    if (koren->faktorRavnoteze == 2 && koren->levo->faktorRavnoteze >= 0) {
        return rotacijaLL(koren);
    }

    if (koren->faktorRavnoteze == 2 && koren->levo->faktorRavnoteze < 0) {
        return rotacijaLR(koren);
    }

    if (koren->faktorRavnoteze == -2 && koren->desno->faktorRavnoteze > 0) {
        return rotacijaRL(koren);
    }
    
    if (koren->faktorRavnoteze == -2 && koren->desno->faktorRavnoteze <= 0) {
        return rotacijaRR(koren);
    }
    
    return koren;
}

inline bool pronadji(Cvor* koren, int x) {
    if (koren == nullptr) {
        return false;
    }

    if (x < koren->kljuc) {
        return pronadji(koren->levo, x);
    }

    if (x > koren->kljuc) {
        return pronadji(koren->desno, x);
    }

    return true;
}

inline Cvor* umetni(Cvor* koren, int x) {
    if (koren == nullptr) {
        return new Cvor{.kljuc = x, .levo = nullptr, .desno = nullptr,
                        .visina = 0, .faktorRavnoteze = 0};
    }

    if (x < koren->kljuc) {
        koren->levo = umetni(koren->levo, x);
    } else if (x > koren->kljuc) {
        koren->desno = umetni(koren->desno, x);
    }

    return balansiraj(koren);
}

inline Cvor* najmanji(Cvor* koren) {
    if (koren->levo == nullptr) {
        return koren;
    }

    return najmanji(koren->levo);
}

inline Cvor* brisi(Cvor* koren, int x) {
    if (koren == nullptr) {
        return nullptr;
    }

    if (x < koren->kljuc) {
        koren->levo = brisi(koren->levo, x);
        return balansiraj(koren);
    }

    if (x > koren->kljuc) {
        koren->desno = brisi(koren->desno, x);
        return balansiraj(koren);
    }

    if (koren->levo == nullptr) {
        Cvor *r = koren->desno;
        delete koren;
        return r;
    }

    if (koren->desno == nullptr) {
        Cvor *l = koren->levo;
        delete koren;
        return l;
    }

    Cvor* sledeci = najmanji(koren->desno);
    koren->kljuc = sledeci->kljuc;
    koren->desno = brisi(koren->desno, sledeci->kljuc);
    
    return balansiraj(koren);
}

inline void ispis(Cvor* koren, int dubina) {
    if (koren == nullptr) {
        return;
    }

    ispis(koren->desno, dubina + 1);
    std::cout << std::string(4*dubina, ' ');
    std::cout << koren->kljuc << '\n';
    ispis(koren->levo, dubina + 1);
}

inline void oslobodi(Cvor *koren) {
    if (koren == nullptr) return;
    oslobodi(koren->levo);
    oslobodi(koren->desno);
    delete koren;
}

// Rezultat: 0 za nullptr; inace +/-H, gde je H = visina + 1.
// Negativan znak znaci da u podstablu postoji makar jedan S-AVL cvor.
// List mora da vrati +1 i ostane neoznacen.
// Dopuniti delove [A]-[E]

int oznaciSAVL(Cvor* x) {
    // [A] Prazno podstablo i ponistavanje stare oznake.
    if(x == nullptr) {
        return 0;
    }

    int l = 0, d = 0;

    // [B] Rekurzivno obradi levo i desno podstablo.
    l = oznaciSAVL(x->levo);
    d = oznaciSAVL(x->desno);

    if (l > 0 && d > 0) {
        // [C] Nijedno podstablo ne sadrzi S-AVL cvor.
        // Da li je tekuci cvor S-AVL? Koliku visinu vratiti?
        if(l == d) {
            x->oznaka = true;
            return -(l+1);
        }

        x->oznaka = false;
        return std::max(l, d) + 1;
    } else if (l < 0 || d < 0) {
        // [D] Postoji S-AVL potomak: prenesi informaciju.
        x->oznaka = false;
        return -(std::max(std::abs(l), std::abs(d)) + 1);
    } else {
        // [E] Bar jedno podstablo je prazno.
        x->oznaka = false;
        return std::max(l, d) + 1;
    }
    return 0; 
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

    std::cout << oznaciSAVL(stablo) << std::endl;
}
