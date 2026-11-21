/* Minimalno čvorno pokrivačko mnoštvo na stablu — ispitivanje da li je zadati skup pokrivač i konstrukcija minimalnog

Ideja zadatka: slično 4. zadatku sa 4. časa - radiš na stablu sa korenom 0 i obilaskom od dece ka 
roditeljima (rbegin() do rend()).
Ovde imaš dva koraka:jePokrivac — proverava u O(n + m) da li svaki čvor v != 0 ima bar jedan 
kraj grane (v, roditelj[v]) u skupu pokrivac.

minimalniPokrivacStabla — bira čvor u pokrivač (slično 4. zadatku, ali ovde možeš da vežbaš i 
izbor samog čvora v kada ni v ni roditelj[v] nisu pokriveni, što takođe daje pokrivač minimalne 
kardinalnosti na stablu).

*/

#include <iostream>
#include <vector>
#include <cassert>

using Cvor = int;
using Graf = std::vector<std::vector<Cvor>>;

void odrediRoditeljeIRedosled(const Graf& graf, std::vector<Cvor>& roditelj, std::vector<Cvor>& redosled) {
    roditelj.assign(graf.size(), -2);
    redosled.clear();
    redosled.reserve(graf.size());
    
    if(graf.empty()) {
        return;
    }

    roditelj[0] = -1;
    redosled.push_back(0);
    
    for(std::size_t i = 0; i < redosled.size(); i++) {
        Cvor v = redosled[i];
        for(Cvor u : graf[v]) {
            if(roditelj[u] == -2) {
                roditelj[u] = v;
                redosled.push_back(u);
            }
        }
    }
}

// vraca true ako `jeUCvoru[x]` definise validan cvorni pokrivac celog stabla
bool jePokrivac(const Graf& graf, const std::vector<Cvor>& roditelj, const std::vector<bool>& jeUPokrivacu) {
    // TODO: Za svaki cvor v od 0 do graf.size() - 1:
    // ako roditelj[v] != -1, proveri da li vazi jeUPokrivacu[v] || jeUPokrivacu[roditelj[v]].
    // Ako za neku granu ne vazi, vrati false.
    for(Cvor v = 0; v < graf.size(); v++) {
        if(roditelj[v] != -1) {
            if(!jeUPokrivacu[v] && !jeUPokrivacu[roditelj[v]]) {
                return false;
            }
        }
    }

    return true;
}

// konstruise jedan minimalni cvorni pokrivac stabla obilaskom odozdo nagore
std::vector<Cvor> minimalniPokrivacStabla(const Graf& graf) {
    // TODO 1: Pozovi odrediRoditeljeIRedosled.
    std::vector<Cvor> roditelj;
    std::vector<Cvor> redosled;
    odrediRoditeljeIRedosled(graf, roditelj, redosled);

    // TODO 2: Napravi niz `pokrivenCvor` (ili `uPokrivacu`) velicine graf.size().
    std::vector<bool> uPokrivacu(graf.size(), false);

    // TODO 3: U obrnutom redosledu `redosled` (od listova ka korenu):
    //         ako v nije koren i grana (v, roditelj[v]) jos nije pokrivena
    //         (nijedan od cvorova v i roditelj[v] nije u resenju),
    //         oznaci izabrani cvor (npr. roditelj[v]) u resenju.
    for(auto it = redosled.rbegin(); it != redosled.rend(); it++) {
        Cvor v = *it;
        if(roditelj[v] != -1 && !uPokrivacu[v] && !uPokrivacu[roditelj[v]]) {
            uPokrivacu[roditelj[v]] = true;
        }
    }

    // TODO 4: Vrati vektor cvorova koji pripadaju resenju.
    std::vector<Cvor> rez;
    for(Cvor v = 0; v < graf.size(); v++) {
        if(uPokrivacu[v]) {
            rez.push_back(v);
        }
    }
    return rez;
}

int main() {
    // Test 1: stablo sa jednim cvorom
    {
        Graf g(1);
        std::vector<Cvor> roditelj, redosled;
        odrediRoditeljeIRedosled(g, roditelj, redosled);

        std::vector<bool> prazno(1, false);
        assert(jePokrivac(g, roditelj, prazno) == true);

        auto pok = minimalniPokrivacStabla(g);
        assert(pok.empty());
    }

    // Test 2: lanac 0 - 1 - 2 - 3 (validan i nevalidan pokrivac)
    {
        Graf g(4);
        g[0].push_back(1); g[1].push_back(0);
        g[1].push_back(2); g[2].push_back(1);
        g[2].push_back(3); g[3].push_back(2);

        std::vector<Cvor> roditelj, redosled;
        odrediRoditeljeIRedosled(g, roditelj, redosled);

        std::vector<bool> dobar = {false, true, false, true};
        std::vector<bool> los = {true, false, false, true};

        assert(jePokrivac(g, roditelj, dobar) == true);
        assert(jePokrivac(g, roditelj, los) == false);

        auto pok = minimalniPokrivacStabla(g);
        std::vector<bool> uPok(4, false);
        for (Cvor x : pok) uPok[x] = true;
        assert(jePokrivac(g, roditelj, uPok) == true);
        assert(pok.size() == 2);
    }

    // Test 3: zvezda sa centrom 0
    {
        Graf g(5);
        for (Cvor v = 1; v < 5; ++v) {
            g[0].push_back(v);
            g[v].push_back(0);
        }

        std::vector<Cvor> roditelj, redosled;
        odrediRoditeljeIRedosled(g, roditelj, redosled);

        auto pok = minimalniPokrivacStabla(g);
        std::vector<bool> uPok(5, false);
        for (Cvor x : pok) uPok[x] = true;
        assert(jePokrivac(g, roditelj, uPok) == true);
        assert(pok.size() == 1);
    }

    // Test 4: razgranato stablo na 6 cvorova
    {
        Graf g(6);
        auto dodajGranu = [&](Cvor u, Cvor v) {
            g[u].push_back(v);
            g[v].push_back(u);
        };
        dodajGranu(0, 1);
        dodajGranu(0, 2);
        dodajGranu(1, 3);
        dodajGranu(1, 4);
        dodajGranu(2, 5);

        std::vector<Cvor> roditelj, redosled;
        odrediRoditeljeIRedosled(g, roditelj, redosled);

        auto pok = minimalniPokrivacStabla(g);
        std::vector<bool> uPok(6, false);
        for (Cvor x : pok) uPok[x] = true;
        assert(jePokrivac(g, roditelj, uPok) == true);
        assert(pok.size() == 2);
    }

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}