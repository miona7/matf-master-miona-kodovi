/* Najveća minimalna težina grane u uparivanju fiksnog (ili maksimalnog) kapaciteta

Ideja zadatka: slično 3. zadatku sa 4. časa - umesto da ograničavaš težinu odozgo (tezina <= prag), 
sada svaka grana ima nosivost / minimalnu dozvoljenu težinu tezina, a ti želiš da zadržiš 
maksimalan mogući broj parova K i maksimizuješ „usko grlo“ (najmanju težinu među granama 
koje ulaze u uparivanje). 
Pored toga, V sada može biti proizvoljan podskup čvorova (ne nužno 0..n1-1). */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

using Cvor = int;
using Graf = std::vector<std::vector<Cvor>>;
using Uparivanje = std::vector<Cvor>;

struct TezinskaGrana {
    Cvor u, v;
    long long tezina;
};

struct RezultatBottleneck {
    long long najtezeUskoGrlo;
    Uparivanje uparivanje;
};

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

int odrediSlojeve(std::vector<int>& sloj, const Uparivanje& uparivanje, const Graf& g, const std::vector<Cvor>& V) {
    sloj = std::vector<int>(g.size(), -1);
    std::queue<Cvor> red;

    for(auto v : V) {
        if(uparivanje[v] == -1) {
            sloj[v] = 0;
            red.push(v);
        }
    }

    int poslednjiSloj = -1;
    while(!red.empty()) {
        Cvor v = red.front();
        red.pop();
          
        if(poslednjiSloj >= 0 && sloj[v] > poslednjiSloj) {
            break;
        }
          
        for(auto u : g[v]) {
            if(uparivanje[u] == -1 && poslednjiSloj == -1) {
                poslednjiSloj = sloj[v];
            }

            if(uparivanje[u] != -1 && sloj[uparivanje[u]] == -1) {
                sloj[uparivanje[u]] = sloj[v] + 1;
                red.push(uparivanje[u]);
            }
        }
    }

    return poslednjiSloj;
}

bool odrediAlternirajuciPut(Cvor v, std::vector<int>& sloj, Uparivanje& uparivanje, int poslednjiSloj, const Graf& g) {
    if(sloj[v] > poslednjiSloj) {
        return false;
    }

    int sledeciSloj = sloj[v] + 1;
    sloj[v] = -1;

    for(auto u : g[v]) {
        if(uparivanje[u] == -1) {
            upari(uparivanje, v, u);
            return true;
        }

        if(sloj[uparivanje[u]] == sledeciSloj) {
            if(odrediAlternirajuciPut(uparivanje[u], sloj, uparivanje, poslednjiSloj, g)) {
                upari(uparivanje, v, u);
                return true;
            }
        }
    }

    return false;
}

Uparivanje optimalnoUparivanje(const Graf& g, const std::vector<Cvor>& V) {
    Uparivanje uparivanje(g.size(), -1);
    
    while(true) {
        std::vector<int> slojevi;
        int poslednjiSloj = odrediSlojeve(slojevi, uparivanje, g, V);
        if(poslednjiSloj == -1) {
            break;
        }
        
        for(Cvor v : V) {
            if(uparivanje[v] == -1) {
                odrediAlternirajuciPut(v, slojevi, uparivanje, poslednjiSloj, g);
            }
        }
    }

    return uparivanje;
}

int brojParova(const Uparivanje& uparivanje) {
    int broj = 0;
    for(Cvor u = 0; u < static_cast<Cvor>(uparivanje.size()); ++u) {
        if(uparivanje[u] > u) {
            ++broj;
        }
    }
    return broj;
}

// Formira podgraf samo od grana cija je tezina >= minTezina i nalazi uparivanje.
Uparivanje uparivanjeSaDonjomGranicom(int n, const std::vector<TezinskaGrana>& grane, const std::vector<Cvor>& V, long long minTezina) {
    // TODO 1: Napravi praznu listu suseda velicine n.
    Graf g(n);

    // TODO 2: Dodaj neusmerene grane samo ako vazi grana.tezina >= minTezina.
    for(const auto& grana : grane) {
        if(grana.tezina >= minTezina) {
            g[grana.u].push_back(grana.v);
            g[grana.v].push_back(grana.u);
        }
    }

    // TODO 3: Vrati optimalnoUparivanje(g, V).
    return optimalnoUparivanje(g, V);
}

// Prvo maksimizuje broj parova K (na celom grafu), a zatim maksimizuje minimalnu tezinu grane u uparivanju od K parova.
RezultatBottleneck maksimalnoUskoGrlo(int n, const std::vector<TezinskaGrana>& grane, const std::vector<Cvor>& V) {
    // TODO 1: Ako nema grana, vrati {0, Uparivanje(n, -1)}.
    if(grane.empty()) {
        return {0, Uparivanje(n, -1)};
    }

    // TODO 2: Izvuci jedinstvene sortirane tezine grana.
    std::vector<long long> tezine;
    for(const auto& grana : grane) {
        tezine.push_back(grana.tezina);
    }

    std::sort(tezine.begin(), tezine.end());
    tezine.erase(std::unique(tezine.begin(), tezine.end()), tezine.end());

    // TODO 3: Izracunaj K = brojParova(uparnjeSaDonjomGranicom(n, grane, V, tezine.front())).
    int K = brojParova(uparivanjeSaDonjomGranicom(n, grane, V, tezine.front()));

    // TODO 4: Binarnom pretragom nad indeksima `tezine` nadji najveci indeks `s`
    //         tako da uparivanjeSaDonjomGranicom(n, grane, V, tezine[s]) ima tacno K parova.
    int l = 0; int d = tezine.size() - 1;
    while(l < d) {
        int s = l + (d - l + 1)/2;
        int brPar = brojParova(uparivanjeSaDonjomGranicom(n, grane, V, tezine[s]));

        if(brPar == K) {
            l = s;
        } else {
            d = s - 1;
        }
    }

    // TODO 5: Vrati {tezine[l], uparivanjeSaDonjomGranicom(n, grane, V, tezine[l])}.
    return {tezine[l], uparivanjeSaDonjomGranicom(n, grane, V, tezine[l])};
}

int main() {
    // Test 1: nema grana
    {
        std::vector<TezinskaGrana> grane;
        std::vector<Cvor> V = {0, 1};
        auto res = maksimalnoUskoGrlo(4, grane, V);
        assert(res.najtezeUskoGrlo == 0);
        assert(brojParova(res.uparivanje) == 0);
    }

    // Test 2: usko grlo 8 uz zadrzanih K = 2 para
    {
        std::vector<TezinskaGrana> grane = {
            {0, 2, 10},
            {0, 3, 3},
            {1, 3, 8}
        };
        std::vector<Cvor> V = {0, 1};
        auto res = maksimalnoUskoGrlo(4, grane, V);
        assert(brojParova(res.uparivanje) == 2);
        assert(res.najtezeUskoGrlo == 8);
    }

    // Test 3: proizvoljan podskup V i prag sa donjom granicom
    {
        std::vector<TezinskaGrana> grane = {
            {1, 0, 5},
            {1, 2, 2},
            {3, 2, 9},
            {3, 4, 7}
        };
        std::vector<Cvor> V = {1, 3};

        auto up7 = uparivanjeSaDonjomGranicom(5, grane, V, 7);
        assert(brojParova(up7) == 1);

        auto res = maksimalnoUskoGrlo(5, grane, V);
        assert(brojParova(res.uparivanje) == 2);
        assert(res.najtezeUskoGrlo == 5);
    }

    // Test 4: jednake tezine grana
    {
        std::vector<TezinskaGrana> grane = {
            {0, 2, 42},
            {1, 3, 42}
        };
        std::vector<Cvor> V = {0, 1};
        auto res = maksimalnoUskoGrlo(4, grane, V);
        assert(brojParova(res.uparivanje) == 2);
        assert(res.najtezeUskoGrlo == 42);
    }

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}