#include <iostream>
#include <vector>

using Cvor = int;
using Graf = std::vector<std::vector<Cvor>>;

// Kodno resenje prema postupku iz zadatka 5.5.
// Pokrivac je skup CVOROVA koji pokriva svaku granu stabla.
// Ovo je implementacija opisanog postupka, a ne prepisani listing iz lekcije.

// Za koren 0 odredjuje roditelje i redosled u kome roditelj dolazi pre dece.
// Obrnut redosled zato obradjuje potomke pre njihovih predaka.
// Iterativni obilazak ne trosi rekurzivni stek na dugackom putu.
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

// Vraca jedan pokrivac grana stabla sa najmanjim brojem cvorova.
std::vector<Cvor> najmanjiPokrivacStabla(const Graf& graf) {
    // ZADATAK: Dopuni samo telo ove funkcije. Obilazak i main vec su dati.
    // TODO 1: Pozovi odrediRoditeljeIRedosled da dobijes roditelje i redosled.
    std::vector<Cvor> roditelj;
    std::vector<Cvor> redosled;
    odrediRoditeljeIRedosled(graf, roditelj, redosled);

    // TODO 2: Napravi oznake koje pamte koji su cvorovi izabrani u pokrivac.
    std::vector<bool> izabran(graf.size(), false);

    // TODO 3: Obradi cvorove OBRNUTIM redosledom (deca pre roditelja).
    // Za cvor v koji nije koren razmotri njegovog roditelja w.
    // Ako nijedan kraj grane (v, w) nije izabran, izaberi roditelja w.
    // Ako je neki kraj vec izabran, ova grana je vec pokrivena.
    for(auto it = redosled.rbegin(); it != redosled.rend(); it++) {
        Cvor v = *it;
        if(roditelj[v] != -1 && !izabran[v] && !izabran[roditelj[v]]) {
            izabran[roditelj[v]] = true;
        }
    }

    // TODO 4: Sakupi izabrane cvorove i vrati njihov spisak.
    // Svaki cvor dodaj najvise jednom. Za stablo sa jednim cvorom vrati prazno.
    // Ne pretrazuj i ne brisi sve grane iznova: cilj je O(n) vreme.
    std::vector<Cvor> res;
    for(Cvor v = 0; v < graf.size(); v++) {
        if(izabran[v]) {
            res.push_back(v);
        }
    }

    return res;
    
    // PRIVREMENO: Povratak omogucava kompajliranje nedovrsenog sablona.
    // return {};
}

int main() {
    int n;
    if(!(std::cin >> n)) {
        return 0;
    }

    Graf graf(n);
    
    for(int i = 0; i < n - 1; i++) {
        Cvor u, v;
        std::cin >> u >> v;
        graf[u].push_back(v);
        graf[v].push_back(u);
    }

    auto pokrivac = najmanjiPokrivacStabla(graf);
    
    std::cout << pokrivac.size() << '\n';
    for(Cvor v : pokrivac) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
    
    return 0;
}
