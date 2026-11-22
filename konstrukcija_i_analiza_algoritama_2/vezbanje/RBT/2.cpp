/* Najduža naizmenična putanja u Crveno-Crnom stablu

Napisati algoritam int najduzaNaizmenicnaPutanja(Cvor* koren) koji u datom crveno-crnom stablu pronalazi dužinu 
najduže proste putanje (merene brojem čvorova) u kojoj se boje susednih čvorova u roditeljsko-dečjoj vezi strogo 
naizmenično smenjuju (tj. nijedan čvor ne sme imati boju istu kao njegov roditelj na toj putanji).

Putanja može početi u bilo kom čvoru (ne mora nužno od korena) i kretati se navani ka potomcima, a može imati i 
dužinu 1 (samo jedan čvor).

Za prazno stablo (nullptr) vratiti 0.

Ograničenja: Vremenska složenost O(n), dodatna memorija O(h) za rekurziju. 
Zabranjeno je menjanje strukture čvorova i pokazivača. */

#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>

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

// vraca par: {dužina puta od korena nadole, globalni maksimum u podstablu}
std::pair<int, int> izracunajNaizmenicnePutanje(Cvor* koren) {
    if(koren == nullptr) {
        return {0, 0};
    }

    auto l = izracunajNaizmenicnePutanje(koren->levo);
    auto d = izracunajNaizmenicnePutanje(koren->desno);
    
    int duzinaOdKorena = 1;
    int maksUStablu = 1;

    int levoOdKorena = 0;
    if(koren->levo != nullptr && koren->boja != koren->levo->boja) {
        levoOdKorena = l.first;
        duzinaOdKorena = std::max(duzinaOdKorena, 1 + levoOdKorena);        
    }

    int desnoOdKorena = 0;
    if(koren->desno != nullptr && koren->boja != koren->desno->boja) {
        desnoOdKorena = d.first;
        duzinaOdKorena = std::max(duzinaOdKorena, 1 + desnoOdKorena);
    }

    // globalni maksimum u ovom podstablu moze biti:
    // 1. najveci maksimum pronađen u levom ili desnom podstablu
    maksUStablu = std::max({maksUStablu, l.second, d.second});

    // 2. putanja koja krece iz korena nadole
    maksUStablu = std::max(maksUStablu, duzinaOdKorena);

    // 3. spojena putanja: levo dete -> koren -> desno dete (ako se boje naizmenicno smenjuju)
    if(koren->levo != nullptr && koren->desno != nullptr &&
        koren->boja != koren->levo->boja && koren->boja != koren->desno->boja) {
        maksUStablu = std::max(maksUStablu, 1 + levoOdKorena + desnoOdKorena);
    }

    return {duzinaOdKorena, maksUStablu};
}

int najduzaNaizmenicnaPutanja(Cvor* koren) {
    if(koren == nullptr) {
        return 0;
    }

    return izracunajNaizmenicnePutanje(koren).second;
}

int main() {
    // 1. Prazno stablo -> 0
    assert(najduzaNaizmenicnaPutanja(nullptr) == 0);

    // 2. Samo jedan čvor -> 1
    Cvor* t1 = new Cvor{.kljuc = 10, .levo = nullptr, .desno = nullptr, .boja = Cvor::Crn};
    assert(najduzaNaizmenicnaPutanja(t1) == 1);
    oslobodi(t1);

    // 3. Otac (Crn) i dete (Crven) -> naizmenično, dužina 2
    //        10 (Crn)
    //       /
    //      5 (Crven)
    Cvor* n5  = new Cvor{.kljuc = 5,  .levo = nullptr, .desno = nullptr, .boja = Cvor::Crven};
    Cvor* t2  = new Cvor{.kljuc = 10, .levo = n5,      .desno = nullptr, .boja = Cvor::Crn};
    assert(najduzaNaizmenicnaPutanja(t2) == 2);
    oslobodi(t2);

    // 4. Isti boja zaredom (prekid naizmeničnosti):
    //        10 (Crn)
    //       /
    //      5 (Crn) -> Ista boja kao otac, putanja se prekida, maksimalna dužina je 1 za svaki čvor posebno.
    Cvor* n5_b = new Cvor{.kljuc = 5,  .levo = nullptr, .desno = nullptr, .boja = Cvor::Crn};
    Cvor* t3   = new Cvor{.kljuc = 10, .levo = n5_b,    .desno = nullptr, .boja = Cvor::Crn};
    assert(najduzaNaizmenicnaPutanja(t3) == 1);
    oslobodi(t3);

    std::cout << "Svi testovi su prošli!\n";
    return 0;
}
