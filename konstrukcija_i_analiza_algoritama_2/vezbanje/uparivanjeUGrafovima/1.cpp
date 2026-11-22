/* Održavanje veličine uparivanja pri uklanjanju grane u gustom / regularnom scenariju

Ideja zadatka: slično 1. zadatku sa 4. časa - mesto da gradiš savršeno uparivanje od nule, 
zamisli da već imaš uparivanje M u grafu i da se jedna grana (a, b) iz M ukloni (ili postane 
neupotrebljiva), čime a i b postaju slobodni čvorovi. Tvoj zadatak je da dopuniš funkciju 
popraviUparivanjePosleBrisanja koja pokušava da ponovo upari oba oslobođena čvora a i b koristeći 
jednu postojeću uparenu granu (u1, u2) iz tekućeg uparivanja (tako da ukupan broj uparenih 
čvorova ostane isti kao pre brisanja). */

#include <iostream>
#include <vector>
#include <cassert>

using Cvor = int;
using Graf = std::vector<std::vector<bool>>;
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

// U ulaznom `uparivanje` cvorovi `a` i `b` su vec slobodni (uparivanje[a] == -1 i uparivanje[b] == -1).
// Vraca true ako je uspelo da oba postanu uparena zamenom jedne postojece grane (u1, u2), inace false.
bool popraviUparivanjePosleBrisanja(const Graf& graf, Uparivanje& uparivanje, Cvor a, Cvor b) {
    // TODO 1: Ako je a == b ili je neki od njih vec uparen, vrati false.
    if(a == b || uparivanje[a] != -1 || uparivanje[b] != -1) {
        return false;
    }

    // TODO 2: Prodji kroz sve cvorove u1 i pronadji njihovog para u2 = uparivanje[u1].
    // Razmatraj svaku uparenu granu (u1, u2) tacno jednom (npr. u1 < u2).
    // Pazi da u1 i u2 budu razliciti od a i b.

    // TODO 3: Proveri da li je moguce spojiti (a, u1) i (b, u2) ILI (a, u2) i (b, u1).
    // Ako jeste, iskoristi funkciju `upari` dva puta da formiras dva nova para i vrati true.
    for(Cvor u1 = 0; u1 < graf.size(); u1++) {
        Cvor u2 = uparivanje[u1];
        if(u2 != -1 && u1 < u2) {
            if(graf[a][u1] && graf[b][u2]) {
                upari(uparivanje, a, u1);
                upari(uparivanje, b, u2);
                return true;
            }

            if(graf[a][u2] && graf[b][u1]) {
                upari(uparivanje, a, u2);
                upari(uparivanje, b, u1);
                return true;
            }
        }
    }

    return false;
}

bool jeValidnoUparivanje(const Graf& graf, const Uparivanje& u) {
    int n = static_cast<int>(graf.size());
    
    if(static_cast<int>(u.size()) != n) {
        return false;
    }
    
    for(int i = 0; i < n; ++i) {
        if(u[i] != -1) {
            int j = u[i];
            if(j < 0 || j >= n || j == i || !graf[i][j] || u[j] != i) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // Test 1: uspesna zamena (0, 2) i (1, 3) preko grane (2, 3)
    {
        Graf g(6, std::vector<bool>(6, false));
        g[0][2] = g[2][0] = true;
        g[1][3] = g[3][1] = true;
        g[2][3] = g[3][2] = true;
        g[4][5] = g[5][4] = true;

        Uparivanje up = {-1, -1, 3, 2, 5, 4};
        assert(popraviUparivanjePosleBrisanja(g, up, 0, 1) == true);
        assert(jeValidnoUparivanje(g, up));
        assert(up[0] != -1 && up[1] != -1);
    }

    // Test 2: uspesna zamena sa obrnutim krajevima (0, 3) i (1, 2)
    {
        Graf g(4, std::vector<bool>(4, false));
        g[0][3] = g[3][0] = true;
        g[1][2] = g[2][1] = true;
        g[2][3] = g[3][2] = true;

        Uparivanje up = {-1, -1, 3, 2};
        assert(popraviUparivanjePosleBrisanja(g, up, 0, 1) == true);
        assert(jeValidnoUparivanje(g, up));
        assert(up[0] == 3 && up[1] == 2);
    }

    // Test 3: samo jedna nova grana postoji -> false
    {
        Graf g(4, std::vector<bool>(4, false));
        g[0][2] = g[2][0] = true;
        g[2][3] = g[3][2] = true;

        Uparivanje up = {-1, -1, 3, 2};
        assert(popraviUparivanjePosleBrisanja(g, up, 0, 1) == false);
        assert(jeValidnoUparivanje(g, up));
    }

    // Test 4: a == b -> false
    {
        Graf g(3, std::vector<bool>(3, false));
        Uparivanje up = {-1, -1, -1};
        assert(popraviUparivanjePosleBrisanja(g, up, 1, 1) == false);
    }

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}