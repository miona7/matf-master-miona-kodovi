// Dopuni [A]-[E]; ne pisi ponovo strukturu Cvor ni main.
// Naredni(x, k) je neuspesan ako x NE POSTOJI u stablu.

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

    int D = 1; // Broj cvorova u podstablu, ukljucujuci ovaj cvor
    void azurirajD(); // Student dopunjava ovu funkciju u zadatku 3.5.

    void azurirajVrednosti() {
        int visinaLevo = levo ? levo->visina : -1;
        int visinaDesno = desno ? desno->visina : -1;
        visina = 1 + std::max(visinaLevo, visinaDesno);
        faktorRavnoteze = visinaLevo - visinaDesno;
        azurirajD(); // Posle promene veza, zajedno sa visinom i faktorom
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
        Cvor *r = koren->desno;
        delete koren;
        return r;
    }

    if(koren->desno == nullptr) {
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
    if(koren == nullptr) {
        return;
    }

    ispis(koren->desno, dubina + 1);
    std::cout << std::string(4*dubina, ' ');
    std::cout << koren->kljuc << '\n';
    ispis(koren->levo, dubina + 1);
}

inline void oslobodi(Cvor *koren) {
    if(koren == nullptr) return;
    oslobodi(koren->levo);
    oslobodi(koren->desno);
    delete koren;
}

int brojCvorova(const Cvor* p) {
    // [A] Vrati 0 za nullptr, inace p->D, u O(1).
    if(p == nullptr) {
        return 0;
    }
    return p->D;
}

void Cvor::azurirajD() {
    // [B] D je broj cvorova u podstablu, ukljucujuci ovaj cvor.
    // Izracunaj ga iz podataka dece u O(1).
    // Poziv iz azurirajVrednosti() vec je dodat, i u rotacijama.
    D = 1 + brojCvorova(levo) + brojCvorova(desno);
}

int rangKljuca(Cvor* koren, int x) {
    // [C] Pronadji x i vrati njegov rang u CELOM stablu (od 1).
    // Ako x ne postoji, vrati 0. Prati samo jedan put od korena.
    
    if(koren == nullptr) {
        return 0;
    }

    // Rang korena: 1 + D(koren->levo).
    Cvor* w = koren;
    int r = 1 + brojCvorova(w->levo);

    while(w != nullptr) {
        if(x == w->kljuc) {
            return r;
        }

        if(x < w->kljuc) {
            // Za wL = w->levo:  Rang(wL) = Rang(w) - D(wL->desno) + 1.
            Cvor* wL = w->levo;
            if(wL == nullptr) {
                return 0;
            }

            r -= brojCvorova(wL->desno) + 1;
            w = wL;
        } else {
            // Za wD = w->desno: Rang(wD) = Rang(w) + D(wD->levo) + 1.
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

Cvor* poRangu(Cvor* koren, int rang) {
    // [D] Pronadji cvor sa zadatim APSOLUTNIM rangom.
    // Za rang van [1, brojCvorova(koren)] vrati nullptr.
    // Poredi trazeni rang sa rangom tekuceg cvora i idi ka jednom detetu.
    // Pri prelasku na dete koristi iste formule kao u [C].
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

Cvor* naredni(Cvor* koren, int x, int k) {
    // [E] Najpre pronadji rang POSTOJECEG kljuca x.
    // Zatim trazi cvor ranga Rang(x) + k pomocu [D].
    // nullptr: x ne postoji, k <= 0 ili nema dovoljno vecih kljuceva.
    // Proveri k <= brojCvorova(koren) - r PRE sabiranja r + k.
    if(k <= 0) {
        return nullptr;
    }

    int r = rangKljuca(koren, x);
    if(r == 0) {
        // x ne postoji
        return nullptr;
    }

    if(k > brojCvorova(koren) - r) {
        return nullptr;
    }

    return poRangu(koren, r + k);
}
