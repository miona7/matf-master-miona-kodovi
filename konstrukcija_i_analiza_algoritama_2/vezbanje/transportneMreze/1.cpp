/* Zadatak: Maksimalni protok uz više izvora i ponora (super-izvor i super-ponor)

U mrežnom protoku često postoji više izvora (npr. skladišta ili elektrane) i više ponora 
(npr. potrošači ili gradovi), dok Edmonds–Karpov algoritam radi sa jednim izvorom s = 0 i 
jednim ponorom t = n - 1.

Ideja zadatka: Umesto da ručno spajaš čvorove u main-u, dopuni funkciju 
maksimalniProtokViseIzvoraIPonora koja na zadati graf sa n čvorova dodaje:

- novi super-izvor (npr. indeks n) sa granama beskonačnog (ili dovoljno velikog) kapaciteta 
ka svakom čvoru iz vektora izvori

- novi super-ponor (npr. indeks n + 1) sa granama takvog kapaciteta iz svakog čvora vektora 
ponori ka njemu.

Zatim pokreni odrediMaksimalanTok na proširenoj mreži i vrati dobijeni maksimalni protok. */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cassert>

using Cvor = int;
using Graf = std::vector<std::vector<Cvor>>;
using Kapacitet = std::vector<std::vector<int>>;

int odrediPovecavajuciPut(std::vector<Cvor>& put, const Kapacitet& kapacitet, const Graf& graf, Cvor s = 0, Cvor t = -1) {
    if(t == -1) {
        t = static_cast<Cvor>(graf.size()) - 1;
    }

    std::vector<Cvor> roditelj(graf.size(), -1);

    std::queue<Cvor> red;
    red.push(s);

    while(!red.empty()) {
        Cvor u = red.front(); red.pop();

        for(auto v : graf[u]) {
            if(roditelj[v] == -1 && kapacitet[u][v] > 0) {
                roditelj[v] = u;

                if(v == t) {
                    break;
                }

                red.push(v);
            }
        }
    }

    if(roditelj[t] == -1) {
        return 0;
    }

    int povecanje = std::numeric_limits<int>::max();
    put = {t};
    
    for(Cvor v = t; roditelj[v] != -1; v = roditelj[v]) {
        povecanje = std::min(povecanje, kapacitet[roditelj[v]][v]);
        put.push_back(roditelj[v]);
    }

    return povecanje;
}

int odrediMaksimalanTok(Kapacitet& kapacitet, const Graf& graf, Cvor s = 0, Cvor t = -1) {
    if(t == -1) {
        t = static_cast<Cvor>(graf.size()) - 1;
    }

    int tok = 0;
    std::vector<Cvor> put;
    int povecanje;

    while((povecanje = odrediPovecavajuciPut(put, kapacitet, graf, s, t)) > 0) {
        tok += povecanje;
        for(size_t i = 1; i < put.size(); i++) {
            Cvor v = put[i-1]; Cvor u = put[i];
            kapacitet[v][u] += povecanje;
            kapacitet[u][v] -= povecanje;
        }
    }

    return tok;
}

int sumaSvihKapaciteta(const Kapacitet& kapacitet) {
    int sum = 0;
    for(int i = 0; i < kapacitet.size(); i++) {
        for(int j = 0; j < kapacitet[i].size(); j++) {
            sum += kapacitet[i][j];
        }
    }
    return sum;
}

// Pretpostavimo da `graf` i `kapacitet` imaju `n` cvorova (0..n-1).
// `izvori` sadrzi indekse pocetnih cvorova, a `ponori` indekse krajnjih cvorova.
int maksimalniProtokViseIzvoraIPonora(int n, const Graf& graf, const Kapacitet& kapacitet,
                                      const std::vector<Cvor>& izvori, const std::vector<Cvor>& ponori) {
    // TODO 1: Napravi prosireni graf i matricu kapaciteta velicine n + 2.
    Graf prosirenGraf(n + 2); 
    Kapacitet prosirenKapacitet(n + 2, std::vector<int>(n + 2, 0));
    
    // Cvor `n` je super-izvor, a cvor `n + 1` je super-ponor.
    // Prekopiraj postojece grane i kapacitete za cvorove 0..n-1.
    for(Cvor v = 0; v < n; v++) {
        for(Cvor u : graf[v]) {
            prosirenGraf[v].push_back(u);
        }
    }
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            prosirenKapacitet[i][j] = kapacitet[i][j];
        }
    }

    // TODO 2: Izracunaj bezbedan veliki kapacitet (npr. sumu svih kapaciteta u mrezi).
    int sum = sumaSvihKapaciteta(kapacitet);

    // TODO 3: Za svaki izvor `s_i` iz `izvori` dodaj granu n -> s_i sa tim kapacitetom
    //         (i proveri kako ti `odrediPovecavajuciPut` i `odrediMaksimalanTok`
    //         koriste povratne/rezidualne grane u `graf` i `kapacitet`).
    for(int i = 0; i < izvori.size(); i++) {
        prosirenGraf[n].push_back(izvori[i]);
        prosirenKapacitet[n][izvori[i]] = sum;
    }

    // TODO 4: Za svaki ponor `t_j` iz `ponori` dodaj granu t_j -> (n + 1) sa tim kapacitetom.
    for(int i = 0; i < ponori.size(); i++) {
        prosirenGraf[ponori[i]].push_back(n+1);
        prosirenKapacitet[ponori[i]][n+1] = sum;
    }

    // TODO 5: Pozovi `odrediMaksimalanTok` na prosirenoj mrezi i vrati rezultat.
    return odrediMaksimalanTok(prosirenKapacitet, prosirenGraf, n, n+1);
}

int main() {
    // Test 1: dva nezavisna toka -> 12
    {
        int n = 4;
        Graf g(n);
        Kapacitet cap(n, std::vector<int>(n, 0));

        g[0].push_back(2); cap[0][2] = 5;
        g[1].push_back(3); cap[1][3] = 7;

        assert(maksimalniProtokViseIzvoraIPonora(n, g, cap, {0, 1}, {2, 3}) == 12);
    }

    // Test 2: usko grlo 2 -> 3 ogranlcava na 6
    {
        int n = 4;
        Graf g(n);
        Kapacitet cap(n, std::vector<int>(n, 0));

        g[0].push_back(2); cap[0][2] = 10;
        g[1].push_back(2); cap[1][2] = 10;
        g[2].push_back(3); cap[2][3] = 6;

        assert(maksimalniProtokViseIzvoraIPonora(n, g, cap, {0, 1}, {3}) == 6);
    }

    // Test 3: vise ponora -> 9
    {
        int n = 4;
        Graf g(n);
        Kapacitet cap(n, std::vector<int>(n, 0));

        g[0].push_back(1); cap[0][1] = 4;
        g[0].push_back(2); cap[0][2] = 5;
        g[1].push_back(3); cap[1][3] = 6;

        assert(maksimalniProtokViseIzvoraIPonora(n, g, cap, {0}, {2, 3}) == 9);
    }

    // Test 4: nema puta do ponora -> 0
    {
        int n = 3;
        Graf g(n);
        Kapacitet cap(n, std::vector<int>(n, 0));

        g[0].push_back(1); cap[0][1] = 10;

        assert(maksimalniProtokViseIzvoraIPonora(n, g, cap, {0}, {2}) == 0);
    }

    std::cout << "Svi testovi su prosli!\n";
    return 0;
}