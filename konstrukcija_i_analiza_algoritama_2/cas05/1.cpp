// Edmonds-Karpov algoritam
#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using Cvor = int;

// graf cuvamo kao listu suseda
using Graf = std::vector<std::vector<Cvor>>;

// kapacitete grana u grafu cuvamo kao matricu
using Kapacitet = std::vector<std::vector<int>>;

// implementira Edmonds-Karp BFS pretragu kojom se pronalazi povecavajuci put u rezidualnom grafu
// put se zapisuje u vektoru put, a funkcija vraca povecanje toka duz tog puta
int odrediPovecavajuciPut(std::vector<Cvor>& put, const Kapacitet& kapacitet, const Graf& graf) {
    // izvor je cvor 0, a ponor je n-1
    int s = 0; int t = graf.size() - 1;

    // za svaki cvor cuvamo njegovog roditelja u pretrazi 
    // ovo nam je potrebno da bismo rekonstruisali put do ponora
    std::vector<Cvor> roditelj(graf.size(), -1);

    // BFS zapocinjemo od izvora s
    std::queue<Cvor> red;
    red.push(s);

    while(!red.empty()) {
        Cvor u = red.front(); red.pop();

        // obilazimo sve susede cvora u
        for(auto v : graf[u]) {
            // ako v nije obidjen i ako ima kapacitet u rezidualnom grafu, obilazimo ga
            if(roditelj[v] == -1 && kapacitet[u][v] > 0) {
                roditelj[v] = u;
                
                // ako smo stigli do ponora prekidamo obilazak
                if(v == t) {
                    break;
                }

                red.push(v);
            }
        }
    }

    // ako nismo pronasli put do ponora vracamo 0
    if(roditelj[t] == -1) {
        return 0;
    }

    // rekonstruisemo put od izvora do ponora i vracamo povecanje toka
    // povecanje toka je minimum kapaciteta grana na tom putu

    int povecanje = std::numeric_limits<int>::max();
    put = {t};
    
    for(Cvor v = t; roditelj[v] != -1; v = roditelj[v]) {
        povecanje = std::min(povecanje, kapacitet[roditelj[v]][v]);
        put.push_back(roditelj[v]);
    }

    return povecanje;
}

// implementira Ford-Fulkerson algoritam za odredjivanje maksimalnog toka u grafu
// u matrici kapacitet se nalaze kapaciteti grana u pocetnom grafu, koja se azurira vrednostima rezidualnog grafa
// vraca vrednost maksimalnog toka
int odrediMaksimalanTok(Kapacitet& kapacitet, const Graf& graf) {
    int tok = 0;
    std::vector<Cvor> put;
    int povecanje;

    while((povecanje = odrediPovecavajuciPut(put, kapacitet, graf)) > 0) {
        tok += povecanje;
        for(size_t i = 1; i < put.size(); i++) {
            Cvor v = put[i-1]; Cvor u = put[i];
            kapacitet[v][u] += povecanje;
            kapacitet[u][v] -= povecanje;
        }
    }

    return tok;
}

int main() {
    int n, m;
    std::cin >> n >> m;

    Graf graf(n);
    Kapacitet kapacitet(n, std::vector<int>(n, 0));

    while(m--) {
        int v, u, w;
        std::cin >> v >> u >> w;

        graf[v].push_back(u);
        kapacitet[v][u] = w;
    }

    Kapacitet rezidualniGraf = kapacitet;
    std::cout << odrediMaksimalanTok(rezidualniGraf, graf) << std::endl;

    for(Cvor u = 0; u < n; u++) {
        for(Cvor v : graf[u]) {
            if(rezidualniGraf[v][u] > 0) {
                std::cout << u << " " << v << " " << rezidualniGraf[v][u] << std::endl;
            }
        }
    }
}