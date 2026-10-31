#include <iostream>
#include <string>
#include <limits>

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

// ZADATAK: Dopuni funkciju. Vrati true akko je dato stablo crveno-crno.
// Vazi: nullptr predstavlja crni NIL list. Ne menjaj nijedan cvor.
// Uslovi: BST poredak u CELOM podstablu, crn koren, bez uzastopnih
// crvenih cvorova, jednake crne visine svih putanja do NIL listova.
// Ocekivana slozenost: O(n) vremena i O(h) memorije za rekurziju.

// Pomoćna funkcija koja vraća crnu visinu podstabla ako je validno, 
// ili -1 ako stablo narušava bilo koje pravilo crveno-crnog stabla.
// Takođe proverava BST uslov (opseg [min_v, max_v] za ključeve).
int validnoCrvenoCrno(Cvor* cvor, int minV, int maxV, bool imaMin, bool imaMax) {
    if(cvor == nullptr) {
        return 0;
    }

    // 1. Provera BST uslova (ključevi moraju biti u dozvoljenom opsegu)
    if(imaMin && cvor->kljuc <= minV) {
        return -1;
    }
    if(imaMax && cvor->kljuc >= maxV) {
        return -1;
    }

    // 2. Provera uslova: ne smeju postojati dva uzastopna crvena čvora
    if(crven(cvor)) {
        if(crven(cvor->levo) || crven(cvor->desno)) {
            return -1;
        }
    }

    // Rekurzivni poziv za levo i desno podstablo sa ažuriranim granicama
    int levaCrnaVisina = validnoCrvenoCrno(cvor->levo, minV, cvor->kljuc, imaMin, true);
    if(levaCrnaVisina == -1) {
        return -1;
    }

    int desnaCrnaVisina = validnoCrvenoCrno(cvor->desno, cvor->kljuc, maxV, true, imaMax);
    if(desnaCrnaVisina == -1) {
        return -1;
    }

    // 3. Provera uslova: sve putanje do NIL listova moraju imati istu crnu visinu
    if(levaCrnaVisina != desnaCrnaVisina) {
        return -1;
    }

    // Ukupna crna visina ovog podstabla jednaka je crnoj visini dece uvećanoj za 1 ako je tekući čvor crn
    return levaCrnaVisina + (crn(cvor) ? 1 : 0);
}

bool validnoCrvenoCrno(Cvor* koren) {
    // TODO: Napisati proveru svih uslova.
    // Podsetnik: nije dovoljno porediti kljuc samo sa neposrednom decom!
    
    if(koren != nullptr && crven(koren)) {
        return false;
    }

    // Pozivamo pomoćnu funkciju koja istovremeno proverava:
    // - BST strukturu (kroz opsege minimalnih i maksimalnih vrednosti)
    // - Odsustvo uzastopnih crvenih čvorova
    // - Jednakost crnih visina na svim putanjama
    return validnoCrvenoCrno(koren, std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), false, false) != -1;
}

// Profesorkin glavni program zadrzan je za samostalnu upotrebu. Testovi
// radionice koriste sopstveni main; zato ga u njima iskljucujemo.
#ifndef RBT_WORKSHOP
int main() {
    Cvor* koren = nullptr;
    int c;
    while(std::cin >> c) {
        koren = umetni(koren, c);
        std::cout << "-------" << std::endl;
        ispis(koren, 0);
        std::cout << "-------" << std::endl;
    }
    return 0;
}
#endif
