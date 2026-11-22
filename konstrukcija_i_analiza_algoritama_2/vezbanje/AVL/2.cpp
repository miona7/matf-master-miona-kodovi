/* Zadatak 2 (sličan zadatku 3 sa 1. casa – prošireno AVL stablo i upiti u O(log n))

Realizovati apstraktni tip podataka nad skupom celih brojeva koristeći AVL stablo koje podržava operacije Umetni(x), 
Obrisi(x), Prethodni(x, k) i ZbirManjihOd(x). Svaka operacija mora da radi u O(log n) vremenu u najgorem slučaju.
U strukturi Cvor pored postojećih polja čuvaju se: 
- int D = 1; — broj čvorova u podstablu (uključujući tekući čvor)
- long long suma; — zbir ključeva svih čvorova u podstablu (uključujući tekući čvor; za novokreirani list važi suma = kljuc).

Potrebno je realizovati:
- void Cvor::azurirajDodatnaPolja() — u O(1) ažurira i D i suma iz podataka dece (poziva se unutar azurirajVrednosti() 
tako da ostane tačno i nakon umetanja, brisanja i svih rotacija)

- Cvor* prethodni(Cvor* koren, int x, int k) — za postojeći ključ x i k >= 1, vraća pokazivač na k-ti najveći ključ koji je
strogo manji od x (npr. za k = 1 vraća neposrednog prethodnika). Ako x ne postoji u stablu, ako je k <= 0 ili ako u stablu 
ima manje od k ključeva manjih od x, vraća nullptr

- long long zbirManjihOd(Cvor* koren, int x) — vraća zbir svih ključeva u stablu koji su strogo manji od x. 
Ključ x ne mora da postoji u stablu. Za prazno stablo ili ako takvih ključeva nema, vraća 0. */

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

    int D = 1; // broj čvorova u podstablu (uključujući tekući čvor)
    long long suma; // zbir ključeva svih čvorova u podstablu (uključujući tekući čvor; za novokreirani list važi suma = kljuc).

    void azurirajDodatnaPolja();

    void azurirajVrednosti() {
        int visinaLevo = levo != nullptr ? levo->visina : -1;
        int visinaDesno = desno != nullptr ? desno->visina : -1;
        visina = 1 + std::max(visinaLevo, visinaDesno);
        faktorRavnoteze = visinaLevo - visinaDesno;

        azurirajDodatnaPolja();
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
                        .visina = 0, .faktorRavnoteze = 0, .suma = x};
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

inline int brojCvorova(Cvor* koren) {
    if(koren == nullptr) {
        return 0;
    }
    return koren->D;
}

inline long long sumaCvora(Cvor* koren) {
    if(koren == nullptr) {
        return 0;
    }
    return koren->suma;
}

void Cvor::azurirajDodatnaPolja() {
    D = 1 + brojCvorova(levo) + brojCvorova(desno);
    suma = kljuc + sumaCvora(levo) + sumaCvora(desno);
}

inline int rang(Cvor* koren, int x) {
    if(koren == nullptr) {
        return 0;
    }

    Cvor* w = koren;
    int r = brojCvorova(w->levo) + 1; // rang tekuceg cvora (broj <= cvorova)

    while(w != nullptr) {
        if(x == w->kljuc) {
            return r;
        } 

        if(x < w->kljuc) {
            Cvor* wL = w->levo;
            if(wL == nullptr) {
                return 0;
            }

            r -= brojCvorova(wL->desno) + 1;
            w = wL;
        } else {
            Cvor* wD = w->desno;
            if(wD == nullptr) {
                return 0;
            }

            r += brojCvorova(wD->levo) + 1;
            w = wD;
        }
    }

    return 0;
}

inline Cvor* poRangu(Cvor* koren, int rang) {
    if(rang < 1 || rang > brojCvorova(koren)) {
        return nullptr;
    }

    Cvor* w = koren;
    int r = brojCvorova(w->levo) + 1; 

    while(w != nullptr) {
        if(r == rang) {
            return w;
        } 

        if(rang < r) {
            Cvor* wL = w->levo;
            if(wL == nullptr) {
                return nullptr;
            }

            r -= brojCvorova(wL->desno) + 1;
            w = wL;
        } else {
            Cvor* wD = w->desno;
            if(wD == nullptr) {
                return nullptr;
            }

            r += brojCvorova(wD->levo) + 1;
            w = wD;
        }
    }

    return nullptr;
}

inline Cvor* prethodni(Cvor* koren, int x, int k) {
    if(koren == nullptr || k <= 0 || k > brojCvorova(koren)) {
        return nullptr;
    }

    Cvor* w = koren;
    int r = brojCvorova(koren->levo) + 1;

    while(w != nullptr) {
        if(x == w->kljuc) {
            return poRangu(koren, r - k);
        }

        if(x < w->kljuc) {
            Cvor* wL = w->levo;
            if(wL == nullptr) {
                return nullptr;
            }

            r -= brojCvorova(wL->desno) + 1;
            w = wL;
        } else {
            Cvor* wD = w->desno;
            if(wD == nullptr) {
                return nullptr;
            }

            r += brojCvorova(wD->levo) + 1;
            w = wD;
        }
    }

    return nullptr;
}

inline long long zbirManjihOd(Cvor* koren, int x) {
    if(koren == nullptr) {
        return 0;
    }

    Cvor* w = koren;
    long long rez = 0;

    while(w != nullptr) {
        if(w->kljuc < x) {
            rez += sumaCvora(w->levo) + w->kljuc;
            w = w->desno;
        } else if(w->kljuc == x) {
            rez += sumaCvora(w->levo);
            return rez;
        } else {
            w = w->levo;
        }
    }

    return rez;
}

int main() {
    Cvor* stablo = nullptr;

    // 1. Prazno stablo
    assert(prethodni(stablo, 10, 1) == nullptr);
    assert(zbirManjihOd(stablo, 10) == 0);

    // 2. Umetanje elemenata: {10, 20, 30, 40, 50, 60, 70}
    // Ukupan zbir = 280, broj cvorova = 7
    for (int x : {40, 60, 20, 30, 10, 70, 50}) {
        stablo = umetni(stablo, x);
    }

    assert(stablo->D == 7);
    assert(stablo->suma == 280);

    // 3. Testovi za prethodni(stablo, x, k)
    assert(prethodni(stablo, 50, 1)->kljuc == 40);
    assert(prethodni(stablo, 50, 2)->kljuc == 30);
    assert(prethodni(stablo, 50, 4)->kljuc == 10);
    assert(prethodni(stablo, 50, 5) == nullptr);  // nema 5 manjih od 50
    assert(prethodni(stablo, 10, 1) == nullptr);  // 10 je najmanji
    assert(prethodni(stablo, 45, 1) == nullptr);  // 45 ne postoji u stablu
    assert(prethodni(stablo, 50, 0) == nullptr);  // k <= 0
    assert(prethodni(stablo, 50, -2) == nullptr); // k < 0

    // 4. Testovi za zbirManjihOd(stablo, x)
    assert(zbirManjihOd(stablo, 10) == 0);        // niko nije strogo manji od 10
    assert(zbirManjihOd(stablo, 5) == 0);         // manji od svih
    assert(zbirManjihOd(stablo, 30) == 30);       // 10 + 20
    assert(zbirManjihOd(stablo, 35) == 60);       // 10 + 20 + 30 (35 nije u stablu)
    assert(zbirManjihOd(stablo, 70) == 210);      // 10 + 20 + 30 + 40 + 50 + 60
    assert(zbirManjihOd(stablo, 100) == 280);     // svi elementi (100 nije u stablu)

    // 5. Brisanje (izaziva rotacije) i ponovna provera konzistentnosti polja D i suma
    // Skup postaje: {10, 20, 30, 50, 70}, ukupan zbir = 180, broj cvorova = 5
    stablo = brisi(stablo, 40);
    stablo = brisi(stablo, 60);

    assert(stablo->D == 5);
    assert(stablo->suma == 180);
    assert(prethodni(stablo, 50, 1)->kljuc == 30);
    assert(prethodni(stablo, 50, 2)->kljuc == 20);
    assert(zbirManjihOd(stablo, 55) == 110);      // 10 + 20 + 30 + 50

    std::cout << "Svi testovi su prosli!\n";
    oslobodi(stablo);
}