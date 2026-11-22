/* Brojanje crvenih čvorova na putu sa najviše crnih čvorova

Napisati algoritam int maxCrvenihNaNajdubljemCrnomPutu(Cvor* koren) koji za dato crveno-crno stablo pronalazi put 
od korena do nekog NIL lista (nullptr) koji ima maksimalnu crnu visinu (tj. najviše crnih čvorova na putu), a kao 
rezultat vraća broj crvenih čvorova na tom specifičnom putu.

Ako postoji više takvih puteva sa maksimalnim brojem crnih čvorova (što je kod ispravnih RBT-a uvek slučaj za crnu visinu,
ali ovde stablo ne mora nužno biti potpuno ispravno), vrati maksimalan broj crvenih čvorova koji se može naći na nekom 
od najdubljih crnih puteva.

Za prazno stablo (nullptr) vratiti 0.

Ograničenja: Vremenska složenost O(n), dodatna memorija O(h) za rekurziju. 
Zabranjeno je menjanje strukture čvorova i pokazivača. */

#include <iostream>
#include <string>
#include <limits>
#include <cassert>

struct Cvor {
    int kljuc;
    Cvor* levo;
    Cvor* desno;
    enum { Crven, Crn } boja;
};

bool crn(Cvor* cvor) { 
    return cvor == nullptr || cvor->boja == Cvor::Crn; 
}

bool crven(Cvor* cvor) { 
    return !crn(cvor); 
}

Cvor* rotacijaLL(Cvor* A) {
    Cvor* B = A->levo;
    A->levo = B->desno;
    B->desno = A;
    return B;
}

Cvor* rotacijaRR(Cvor* A) {
    Cvor* B = A->desno;
    A->desno = B->levo;
    B->levo = A;
    return B;
}

bool pronadji(Cvor* tree, int x) {
    if(tree == nullptr) {
        return false;
    }
    if(x < tree->kljuc) {
        return pronadji(tree->levo, x);
    }
    if(x > tree->kljuc) {
        return pronadji(tree->desno, x);
    }
    return true;
}

// Popravlja invarijantu crveno-crnog stabla nakon umetanja novog cvora.
// Zapocinjemo popravku od dede dodatog cvora i pretpostavljamo da je otac levo.
// Vraca novi koren podstabla.
Cvor* popraviLevo(Cvor* deda) {
    // Uvodimo promenljive zbog citljivosti.
    auto otac = deda->levo;
    auto stric = deda->desno;

    // 1. Slucaj - otac i stric su crveni.
    if(crven(otac) && crven(stric)) {
        if(crven(otac->levo) || crven(otac->desno)) {
            deda->boja = Cvor::Crven;
            otac->boja = stric->boja = Cvor::Crn;
        }
    } else if(crven(otac) && crn(stric)) {
        // 2. Slucaj - otac je crven, stric crn, crveni cvor je desno dete oca.
        if(crven(otac->desno)) {
            otac = deda->levo = rotacijaRR(otac);
        }
        // 3. Slucaj - otac je crven, stric crn, crveni cvor je levo dete oca.
        if(crven(otac->levo)) {
            otac->boja = Cvor::Crn;
            deda->boja = Cvor::Crven;
            deda = rotacijaLL(deda);
        }
    }
    return deda;
}

// Popravlja invarijantu nakon umetanja kada je otac desno dete dede.
Cvor* popraviDesno(Cvor* deda) {
    auto otac = deda->desno;
    auto stric = deda->levo;
    if(crven(otac) && crven(stric)) {
        if(crven(otac->levo) || crven(otac->desno)) {
            deda->boja = Cvor::Crven;
            otac->boja = stric->boja = Cvor::Crn;
        }
    } else if(crven(otac) && crn(stric)) {
        if(crven(otac->levo)) {
            otac = deda->desno = rotacijaLL(otac);
        }
        if(crven(otac->desno)) {
            otac->boja = Cvor::Crn;
            deda->boja = Cvor::Crven;
            deda = rotacijaRR(deda);
        }
    }
    return deda;
}

// Umece novi cvor sa kljucem x i vraca novi koren podstabla.
Cvor* _umetni(Cvor* koren, int x) {
    if(koren == nullptr) {
        return new Cvor{
            .kljuc = x, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crven};
    }
    if(x < koren->kljuc) {
        koren->levo = _umetni(koren->levo, x);
        return popraviLevo(koren);
    } else if(x > koren->kljuc) {
        koren->desno = _umetni(koren->desno, x);
        return popraviDesno(koren);
    }
    return koren;
}

// Nakon dodavanja koren uvek ostaje crn.
Cvor* umetni(Cvor* koren, int x) {
    koren = _umetni(koren, x);
    koren->boja = Cvor::Crn;
    return koren;
}

void oslobodi(Cvor* koren) {
    if(koren == nullptr) {
        return;
    }
    oslobodi(koren->levo);
    oslobodi(koren->desno);
    delete koren;
}

// Ispisuje stablo sa korenom levo, a desnim podstablom iznad njega.
void ispis(Cvor* koren, int dubina) {
    if(koren == nullptr) {
        return;
    }
    ispis(koren->desno, dubina + 1);
    std::cout << std::string(4 * dubina, ' ');
    if(crven(koren)) {
        std::cout << "\033[31m";
    }
    std::cout << koren->kljuc;
    if(crven(koren)) {
        std::cout << "\033[0m";
    }
    std::cout << std::endl;
    ispis(koren->levo, dubina + 1);
}

int maxBrojCrnihCvorova(Cvor* koren, int& maxCrvenih) {
    if(koren == nullptr) {
        maxCrvenih = 0;
        return 1;
    }

    int crveniLevo;
    int crniLevo = maxBrojCrnihCvorova(koren->levo, crveniLevo);

    int crveniDesno;
    int crniDesno = maxBrojCrnihCvorova(koren->desno, crveniDesno);

    if(crniLevo >= crniDesno) {
        maxCrvenih = crveniLevo + (crven(koren) ? 1 : 0);
        return crniLevo + (crn(koren) ? 1 : 0);
    } else {
        maxCrvenih = crveniDesno + (crven(koren) ? 1 : 0);
        return crniDesno + (crn(koren) ? 1 : 0);
    }

    return 0;
}

int maxCrvenihNaNajdubljemCrnomPutu(Cvor* koren) {
    if(koren == nullptr) {
        return 0;
    }

    int maxCrvenih;
    maxBrojCrnihCvorova(koren, maxCrvenih);
    return maxCrvenih;
}
 
int main() {
    // 1. Prazno stablo -> 0 crvenih čvorova
    assert(maxCrvenihNaNajdubljemCrnomPutu(nullptr) == 0);

    // 2. Samo koren (crn, jer koren uvek mora biti crn u validnom RBT-u)
    //        10 (Crn)
    Cvor* t1 = new Cvor{.kljuc = 10, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crn};
    assert(maxCrvenihNaNajdubljemCrnomPutu(t1) == 0);
    oslobodi(t1);

    // 3. Koren (Crn) sa jednim crvenim levim detetom
    //        20 (Crn)
    //       /
    //     10 (Crven)
    Cvor* l1 = new Cvor{.kljuc = 10, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crven};
    Cvor* t2 = new Cvor{.kljuc = 20, .levo = l1,      .desno = nullptr, .boja = Cvor::Crn};
    // Putanja do levog NIL lista ima 1 crni (20) i 1 crveni (10) čvor. Ukupno 1 crveni.
    assert(maxCrvenihNaNajdubljemCrnomPutu(t2) == 1);
    oslobodi(t2);

    // 4. Stablo sa različitim brojem crvenih čvorova na putevima:
    //          30 (Crn)
    //         /   \
    //     20(Crv)  40(Crv)
    //     /          \
    //  10(Crn)      50(Crn)
    //
    // Putevi od korena do NIL listova:
    // - Put 1: 30(Crn) -> 20(Crv) -> 10(Crn)  [Crna visina = 2, broj crvenih = 1]
    // - Put 2: 30(Crn) -> 40(Crv) -> 50(Crn)  [Crna visina = 2, broj crvenih = 1]
    // (Svi validni putevi imaju istu crnu visinu i isti broj crvenih čvorova)
    Cvor* n10 = new Cvor{.kljuc = 10, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crn};
    Cvor* n20 = new Cvor{.kljuc = 20, .levo = n10,     .desno = nullptr, .boja = Cvor::Crven};
    Cvor* n50 = new Cvor{.kljuc = 50, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crn};
    Cvor* n40 = new Cvor{.kljuc = 40, .levo = nullptr, .desno = n50,     .boja = Cvor::Crven};
    Cvor* t3  = new Cvor{.kljuc = 30, .levo = n20,     .desno = n40,     .boja = Cvor::Crn};

    assert(maxCrvenihNaNajdubljemCrnomPutu(t3) == 1);
    oslobodi(t3);

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}
