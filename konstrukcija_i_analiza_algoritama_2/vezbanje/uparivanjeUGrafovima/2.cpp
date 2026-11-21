/* Kuhn-ov algoritam sa prioritetom manjeg indeksa suseda

Ideja zadatka: slično 2. zadatku sa 4. časa - radiš DFS povećavajući put u bipartitnom grafu, 
ali ovog puta se traži deterministički prolazak: kada čvor u bira suseda v, treba da razmotri 
susede u rastućem redosledu indeksa (0, 1, 2, ...). 

Dodatno, napiši pomoćnu funkciju velicinaUparivanja koja vraća broj grana (parova) u 
dobijenom uparivanju. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using Cvor = int;
using Graf = std::vector<std::vector<Cvor>>;
using Uparivanje = std::vector<Cvor>;

void upari(Uparivanje& uparivanje, Cvor u, Cvor v) {
    Cvor uPar = uparivanje[u], vPar = uparivanje[v];

    if(uPar != -1) {
      uparivanje[uPar] = -1;
    }

    if(vPar != -1) {
      uparivanje[vPar] = -1;
    }

    uparivanje[u] = v;
    uparivanje[v] = u;
}

bool dfsAlternirajuci(Cvor u, std::vector<bool>& posecen, Uparivanje& uparivanje, const Graf& graf) {
    // TODO 1: Obradi slucaj kada je `u` vec posecen i oznaci `u` kao posecenog.
    if(posecen[u]) {
        return false;
    }

    posecen[u] = true;

    // TODO 2: Kopiraj listu suseda cvora `u` i sortiraj je rastuce da bi pretraga bila deterministicka.
    auto listaSuseda = graf[u];
    std::sort(listaSuseda.begin(), listaSuseda.end());

    // TODO 3: Za svakog suseda `v`:
    // - ako je `v` slobodan, pozovi `upari(uparivanje, u, v)` i vrati true;
    // - ako je `v` rekurzivno pozovi dfsAlternirajuci nad `uparivanje[v]`,
    //   i ako uspe, pozovi `upari(uparivanje, u, v)` i vrati true.
    for(Cvor v : listaSuseda) {
        if(uparivanje[v] == -1) {
            upari(uparivanje, u, v);
            return true;
        }

        if(dfsAlternirajuci(uparivanje[v], posecen, uparivanje, graf)) {
            upari(uparivanje, u, v);
            return true;
        }
    }

    return false;
}

int velicinaUparivanja(const Uparivanje& uparivanje) {
    // TODO: Vrati ukupan broj uparenih parova (svaki par broji se jednom).

    int rez = 0;
    for(size_t i = 0; i < uparivanje.size(); i++) {
        if(uparivanje[i] != -1) {
            rez++;
        }
    }

    return rez / 2;
}

int main() {
    // Test 1: povecavajuci put 0 - 2 - 1 - 3
    {
        Graf g(4);
        g[0].push_back(2); g[2].push_back(0);
        g[2].push_back(1); g[1].push_back(2);
        g[1].push_back(3); g[3].push_back(1);

        Uparivanje up = {-1, 2, 1, -1};
        std::vector<bool> posecen(4, false);

        assert(dfsAlternirajuci(0, posecen, up, g) == true);
        assert(up[0] == 2 && up[2] == 0);
        assert(up[1] == 3 && up[3] == 1);
        assert(velicinaUparivanja(up) == 2);
    }

    // Test 2: deterministicki izbor manjeg suseda (2 pre 3)
    {
        Graf g(4);
        g[0].push_back(3); g[3].push_back(0);
        g[0].push_back(2); g[2].push_back(0);

        Uparivanje up = {-1, -1, -1, -1};
        std::vector<bool> posecen(4, false);

        assert(dfsAlternirajuci(0, posecen, up, g) == true);
        assert(up[0] == 2 && up[2] == 0);
        assert(up[3] == -1);
        assert(velicinaUparivanja(up) == 1);
    }

    // Test 3: nema puta do slobodnog cvora -> false
    {
        Graf g(3);
        g[0].push_back(1); g[1].push_back(0);
        g[1].push_back(2); g[2].push_back(1);

        Uparivanje up = {-1, 2, 1};
        std::vector<bool> posecen(3, false);

        assert(dfsAlternirajuci(0, posecen, up, g) == false);
        assert(up[1] == 2 && up[2] == 1 && up[0] == -1);
        assert(velicinaUparivanja(up) == 1);
    }

    // Test 4: vec posecen cvor -> false
    {
        Graf g(2);
        g[0].push_back(1); g[1].push_back(0);

        Uparivanje up = {-1, -1};
        std::vector<bool> posecen(2, false);
        posecen[0] = true;

        assert(dfsAlternirajuci(0, posecen, up, g) == false);
        assert(up[0] == -1 && up[1] == -1);
        assert(velicinaUparivanja(up) == 0);
    }

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}