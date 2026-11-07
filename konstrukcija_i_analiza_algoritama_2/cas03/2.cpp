#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>

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
bool nadji(Cvor *cvor, int kljuc) {
    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while(poredi(cvor->sledeci[nivo], kljuc) < 0) {
            cvor = cvor->sledeci[nivo];
        }
    }
    return poredi(cvor->sledeci[0], kljuc);
}

// umece novi cvor sa zadatim kljucem u skip listu
void umetni(Cvor* cvor, int kljuc) {
    std::vector<Cvor *> prethodni(brojNivoa);

    for(int nivo = brojNivoa - 1; nivo >= 0; nivo--) {
        while (poredi(cvor->sledeci[nivo], kljuc) < 0) {
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
    if (cvor == nullptr) {
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

int main() {
    Cvor* koren = novaSkipLista(brojNivoa);
    
    int c;
    
    while(std::cin >> c) {
        if(c >= 0) {
            umetni(koren, c);
        } else {
            brisi(koren, -c);
        }

        std::cout << "-------" << std::endl;
        
        ispis(koren, brojNivoa);
        
        std::cout << "-------" << std::endl;
    }

    return 0;
}