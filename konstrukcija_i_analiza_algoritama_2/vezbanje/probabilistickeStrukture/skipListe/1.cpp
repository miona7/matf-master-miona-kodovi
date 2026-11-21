/* Zadatak 2 (Skip lista – Pronalaženje maksimuma i brisanje opsega)

Nad datom strukturom skip liste realizuj sledeće dve funkcije:

- int pronadjiMaksimum(Cvor* koren) — vraća najveći ključ u skip listi. Ako je lista prazna (tj. u njoj se nalazi 
samo sentinela sa ključem 0 koja nema dalje elemente na nivou 0), funkcija vraća INT_MIN.

- void obrisiOpseg(Cvor* koren, int minV, int maxV) — pronalazi i briše iz skip liste sve čvorove čiji se
ključevi nalaze u zatvorenom intervalu [minV, maxV]. Operacija treba efikasno da preskače obrisane elemente na 
svim nivoima (možeš iskoristiti logiku sličnu postojećoj funkciji brisi za svaki ključ iz tog opsega, ili optimizovati 
prolazak kroz listu). */

#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <limits>

const int brojNivoa = 4;
const double p = 0.5;

struct Cvor {
    int kljuc;
    std::vector<Cvor*> sledeci;
};

Cvor* novaSkipLista(int brojNivoa) {
    return new Cvor{0, std::vector<Cvor*>(brojNivoa)};
}

// odredjuje slucajan broj iz intervala [0, 1)
double slucajanBroj() { 
    return rand() / (RAND_MAX + 1.0);
}

// dredjuje slucajan nivo za novi cvor
int slucajanNivo() {
    int nivo = 1;

    while(nivo < brojNivoa && slucajanBroj() < p) {
        nivo++;
    }

    return nivo;
}

// funkcija za poredjenje kljuca cvora sa zadatim kljucem. Vraca:
// - negativan broj ako je kljuc cvora manji,
// - nula ako su kljucevi jednaki,
// - pozitivan broj ako je kljuc cvora veci.
// ako je cvor null, smatra se da je njegov kljuc veci od bilo kog kljuca
int poredi(Cvor* cvor, int kljuc) {
    return (cvor == nullptr) ? 1 : (cvor->kljuc - kljuc);
}

// odredjuje da li u skip listi postoji cvor sa zadatim kljucem
bool nadji(Cvor* cvor, int kljuc) {
    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while(poredi(cvor->sledeci[nivo], kljuc) < 0) {
            cvor = cvor->sledeci[nivo];
        }
    }
    return poredi(cvor->sledeci[0], kljuc) == 0;
}

// umece novi cvor sa zadatim kljucem u skip listu
void umetni(Cvor* cvor, int kljuc) {
    std::vector<Cvor*> prethodni(brojNivoa);

    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while(poredi(cvor->sledeci[nivo], kljuc) < 0) {
            cvor = cvor->sledeci[nivo];
        }
        prethodni[nivo] = cvor;
    }
    
    if(poredi(cvor->sledeci[0], kljuc) == 0) {
        return;
    }

    auto novi = new Cvor{kljuc, std::vector<Cvor*>(slucajanNivo())};
    
    for(int nivo = 0; nivo < novi->sledeci.size(); nivo++) {
        novi->sledeci[nivo] = prethodni[nivo]->sledeci[nivo];
        prethodni[nivo]->sledeci[nivo] = novi;
    }
}

// brise cvor sa zadatim kljucem iz skip liste
void brisi(Cvor* cvor, int kljuc) {
    std::vector<Cvor*> prethodni(brojNivoa);
    
    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while(poredi(cvor->sledeci[nivo], kljuc) < 0) {
            cvor = cvor->sledeci[nivo];
        }
        prethodni[nivo] = cvor;
    }

    if(poredi(cvor->sledeci[0], kljuc) > 0) {
        return;
    }

    auto pronadjen = cvor->sledeci[0];
    
    for(int nivo = 0; nivo < pronadjen->sledeci.size(); nivo++) {
        prethodni[nivo]->sledeci[nivo] = pronadjen->sledeci[nivo];
    }

    delete pronadjen;
}

// ispisuje skip listu. Lista se ispisuje odozgo na dole i u svakom redu ispisani su svi nivoi datog cvora
void ispis(Cvor* cvor, int brojNivoa) {
    if(cvor == nullptr) {
        return;
    }

    std::cout << std::setw(4) << cvor->kljuc << " ";
    
    for(size_t i = 1; i < cvor->sledeci.size(); i++) {
        std::cout << std::setw(4);
        
        if(cvor->sledeci[i] != nullptr) {
            std::cout << cvor->sledeci[i]->kljuc << " ";
        } else {
            std::cout << "X" << " ";    
        }
    }

    for(size_t i = 0; i < brojNivoa - cvor->sledeci.size(); i++) {
        std::cout << std::setw(4) << "|" << " ";
    }
    
    std::cout << std::endl;
    std::cout << std::setw(4) << "|" << " ";
    
    for(size_t i = 0; i < brojNivoa - 1; i++) {
        std::cout << std::setw(4) << "|" << " ";
    }

    std::cout << std::endl;

    ispis(cvor->sledeci[0], brojNivoa);
}

int pronadjiMaksimum(Cvor* koren) {
    Cvor* cvor = koren;

    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while(poredi(cvor->sledeci[nivo], std::numeric_limits<int>::max()) < 0) {
            cvor = cvor->sledeci[nivo];
        }
    }

    if(cvor == koren) {
        return std::numeric_limits<int>::min();
    }

    return cvor->kljuc;
}

void obrisiOpseg(Cvor* koren, int minV, int maxV) {
    if(koren == nullptr) {
        return;
    }

    // mozemo proci kroz listu na nivou 0 i za svaki element ciji je kljuc u opsegu [minV, maxV] pozvati brisanje
    Cvor* cvor = koren->sledeci[0];
    while(cvor != nullptr) {
        int id = cvor->kljuc;
        cvor = cvor->sledeci[0]; // sacuvamo sledeci pre brisanja

        if(id >= minV && id <= maxV) {
            brisi(koren, id);
        } else if(id > maxV) {
            // posto je lista sortirana, cim pređemo maxV nema potrebe da idemo dalje
            break;
        }
    }
}

int main() {
    Cvor* koren = novaSkipLista(brojNivoa);

    // 1. Test prazne liste
    assert(pronadjiMaksimum(koren) == std::numeric_limits<int>::min());

    // 2. Umetanje elemenata: {10, 20, 30, 40, 50, 60, 70}
    for (int x : {40, 20, 60, 10, 30, 70, 50}) {
        umetni(koren, x);
    }

    // 3. Test pronalaženja maksimuma
    assert(pronadjiMaksimum(koren) == 70);

    // 4. Test brisanja opsega [30, 50]
    // Nakon brisanja, u listi treba da ostanu: {10, 20, 60, 70}
    obrisiOpseg(koren, 30, 50);

    assert(pronadjiMaksimum(koren) == 70); // Maksimum je i dalje 70
    assert(nadji(koren, 40) == false);    // 40 je obrisan
    assert(nadji(koren, 30) == false);    // 30 je obrisan
    assert(nadji(koren, 50) == false);    // 50 je obrisan
    assert(nadji(koren, 20) == true);     // 20 je ostao

    // 5. Brisanje preostalih elemenata
    obrisiOpseg(koren, 1, 100);
    assert(pronadjiMaksimum(koren) == std::numeric_limits<int>::min());

    std::cout << "Svi test primeri za Skip listu su uspesno prosli!\n";
    
    // Oslobađanje memorije sentinela
    delete koren;
    
    return 0;
}