/* Zadatak 1 (sličan zadatku 2 sa 1.casa – rekurzivni obilazak i kodiranje povratne vrednosti)

Napisati algoritam int oznaciNajdubljeNeuravnotezene(Cvor* koren) koji u proizvoljnom binarnom stablu 
(ne mora biti AVL) označava sve čvorove x za koje važi da su strogo neuravnoteženi (apsolutna razlika visina 
njihovog levog i desnog podstabla je >= 2), ali nijedan njihov pravi potomak nije strogo neuravnotežen.
Postavi oznaka = true samo kod traženih čvorova, a kod svih ostalih false.
Ne menjaj ključeve, veze, sačuvane visine ni faktore ravnoteže u strukturi Cvor (čak i ako su postojeći podaci u visina 
i faktorRavnoteze neažurni, algoritam mora da radi ispravno).

Povratna vrednost funkcije:
- Za prazno stablo (nullptr) vraća 0.
- Inače vraća +/- H, gde je H = h + 1 (h je stvarna visina podstabla u broju grana, tako da list vraća +1).
Negativan znak znači da u tom podstablu (uključujući njegov koren) postoji makar jedan strogo neuravnotežen čvor; 
pozitivan znak znači da u celom podstablu nema nijednog strogo neuravnoteženog čvora.
Vremenska složenost: O(n), bez dodatne memorije osim steka rekurzije. */

#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

struct Cvor {
    int kljuc;
    Cvor* levo;
    Cvor* desno;
    int visina;
    int faktorRavnoteze;
    bool oznaka = false;

    void azurirajVrednosti() {
        int visinaLevo = levo != nullptr ? levo->visina : -1;
        int visinaDesno = desno != nullptr ? desno->visina : -1;
        visina = 1 + std::max(visinaLevo, visinaDesno);
        faktorRavnoteze = visinaLevo - visinaDesno;
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
                        .visina = 0, .faktorRavnoteze = 0};
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

inline int oznaciNajdubljeNeuravnotezene(Cvor* koren) {
    if(koren == nullptr) {
        return 0; // prazno stablo
    }

    koren->oznaka = false; // resetujemo oznaku

    if(koren->levo == nullptr && koren->desno == nullptr) {
        return 1; // list
    }

    int visinaLevo = oznaciNajdubljeNeuravnotezene(koren->levo);
    int visinaDesno = oznaciNajdubljeNeuravnotezene(koren->desno);

    if(visinaLevo >= 0 && visinaDesno >= 0) { // nijedan pravi potomak nije strogo neuravnotezen
        int H = std::max(visinaLevo, visinaDesno) + 1;
        
        if(std::abs(visinaLevo - visinaDesno) >= 2) { 
            koren->oznaka = true; // trenutni cvor je neuravnotezen
            return -H; // propagiramo neuravnotezenost
        }

        return H; // propagiramo visinu 
    } else { // postoji potomak koji je neuravnotezen u nekom od podstabala
        int H = std::max(std::abs(visinaLevo), std::abs(visinaDesno)) + 1;
        return -H; // propagiramo neuravnotezenost
    }

    return 0; 
}

int main() {
    // 1. Prazno stablo
    assert(oznaciNajdubljeNeuravnotezene(nullptr) == 0);

    // 2. Samo jedan cvor (list)
    Cvor* jedan = new Cvor{.kljuc = 10, .levo = nullptr, .desno = nullptr,
                           .visina = 0, .faktorRavnoteze = 0, .oznaka = true};
    assert(oznaciNajdubljeNeuravnotezene(jedan) == 1);
    assert(jedan->oznaka == false);
    oslobodi(jedan);

    // 3. Savrseno uravnotezeno AVL stablo
    //        20
    //       /  \
    //     10    30
    Cvor* avl = nullptr;
    avl = umetni(avl, 20);
    avl = umetni(avl, 10);
    avl = umetni(avl, 30);
    avl->oznaka = true; // lazna stara oznaka
    assert(oznaciNajdubljeNeuravnotezene(avl) == 2); // H = h + 1 = 1 + 1 = 2
    assert(avl->oznaka == false);
    assert(avl->levo->oznaka == false);
    assert(avl->desno->oznaka == false);
    oslobodi(avl);

    // 4. Slozenije neuravnotezeno stablo (rucno povezano):
    //             50                (visine: L=3, D=0 -> |3 - (-1)| = 4 >= 2, ali ima neuravnotezene potomke -> false)
    //            /  \
    //          30    60             (30: visine L=2, D=-1 -> |2 - (-1)| = 3 >= 2, ali 20 je neuravnotezen -> false)
    //         /
    //       20                      (20: visine L=1, D=-1 -> |1 - (-1)| = 2 >= 2, nijedan potomak nije -> TRUE!)
    //      /
    //    10                         (10: visine L=0, D=-1 -> |0 - (-1)| = 1 < 2 -> false)
    //   /
    //  5                            (5: list -> false)
    Cvor* n5  = new Cvor{.kljuc = 5,  .levo = nullptr, .desno = nullptr};
    Cvor* n10 = new Cvor{.kljuc = 10, .levo = n5,      .desno = nullptr};
    Cvor* n20 = new Cvor{.kljuc = 20, .levo = n10,     .desno = nullptr};
    Cvor* n30 = new Cvor{.kljuc = 30, .levo = n20,     .desno = nullptr, .oznaka = true}; // lazna stara oznaka
    Cvor* n60 = new Cvor{.kljuc = 60, .levo = nullptr, .desno = nullptr, .oznaka = true}; // lazna stara oznaka
    Cvor* n50 = new Cvor{.kljuc = 50, .levo = n30,     .desno = n60};

    // Visina h stabla sa korenom u 50 je 4 (putanja 50-30-20-10-5 ima 4 grane), pa je H = 5.
    // Postoji neuravnotezen cvor -> povratna vrednost mora biti -5.
    int rez = oznaciNajdubljeNeuravnotezene(n50);
    assert(rez == -5);

    assert(n50->oznaka == false);
    assert(n60->oznaka == false);
    assert(n30->oznaka == false);
    assert(n20->oznaka == true);  // jedini najdublji strogo neuravnotezeni cvor
    assert(n10->oznaka == false);
    assert(n5->oznaka == false);

    oslobodi(n50);

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}