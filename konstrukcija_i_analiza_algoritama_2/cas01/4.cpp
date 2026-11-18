#include <algorithm>

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

// LL: jednostruka desna rotacija oko A.
inline Cvor* rotacijaLL(Cvor* A) {
    Cvor* B = A->levo;
    A->levo = B->desno;
    B->desno = A;
    A->azurirajVrednosti();  // A je sada ispod B.
    B->azurirajVrednosti();
    return B;
}

// Cvor i rotacijaLL(Cvor*) su već dati u osnovnom kodu.
// Dopuni samo ovu funkciju. Ne piši main.
// Ulaz: proizvoljno BST, ne nužno AVL. Rezultat: desna lista.
// Čuvaju se svi postojeći čvorovi i njihovi ključevi.
// Ne pravi nove čvorove i ne pozivaj balansiraj/umetni/brisi.
Cvor* kreirajListu(Cvor* koren) {
    // [A] Pripremi način da možeš da promeniš i koren i desni pokazivač prethodnog čvora u listi.
    Cvor** veza = &koren;

    while(*veza != nullptr) {
        Cvor* tmp = *veza;
        if(tmp->levo != nullptr) {
            // [B] Desna rotacija oko tmp.
            // Sačuvaj vraćeni koren preko pokazivača veza.
            *veza = rotacijaLL(tmp);

            // Ostani na istom mestu, pa ponovo proveri levo dete.
            // break; // Privremeno: zameni svojom dopunom.

        } else {
            // [C] Tekući čvor je gotov: pređi na njegovo desno podstablo.
            // break; // Privremeno: zameni svojom dopunom.
            veza = &tmp->desno;
        }
    }
    // [D] Vrati novu glavu liste (koren može biti promenjen).
    return koren;
}

