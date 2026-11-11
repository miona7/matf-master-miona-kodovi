#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using Cvor = int;

// Graf cuvamo kao listu suseda
using Graf = std::vector<std::vector<Cvor>>;

// Za svaki cvor cuvamo njegov par u uparivanju, odnosno -1 ako nije uparen
using Uparivanje = std::vector<Cvor>;

// Uparuje cvorove u i v u uparivanju, rasparuje prethodne parove
void upari(Uparivanje& uparivanje, Cvor u, Cvor v) {
    Cvor uPar = uparivanje[u], vPar = uparivanje[v];

    // Rasparujemo uPar od u i vPar od v (ako postoje)
    if(uPar != -1) {
        uparivanje[uPar] = -1;
    }

    if(vPar != -1) {
        uparivanje[vPar] = -1;
    }

    // Uparujemo u sa v
    uparivanje[u] = v;
    uparivanje[v] = u;
}

// BFS pretraga kojom se odredjuje slojeviti graf.
// Za svaki obidjen cvor se belezi njegov sloj (udaljenost od pocetnog skupa cvorova). 
// Pretraga pocinje iz neuparenih cvorova skupa V. Vraca broj poslednjeg sloja koji nema neuparene cvorove.
int odrediSlojeve(std::vector<int>& sloj, const Uparivanje& uparivanje, const Graf& g, const std::vector<Cvor>& V) {
    // Belezimo kom sloju pripada koji cvor (ako cvor nije obidjen sloj mu je -1)
    sloj = std::vector<int>(g.size(), -1);
    std::queue<Cvor> red;

    // Zapocinjemo pretragu od neuparenih cvorova iz V
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
        // Ako imamo visak cvorova u redu iz slojeva nakon poslednjeg, preskacemo ih
        // Ovo je samo optimizacija da ranije prestanemo sa pretragom, ne utice na slozenost
          
        if(poslednjiSloj >= 0 && sloj[v] > poslednjiSloj) {
            break;
        }
          
        // Obilazimo sve susede cvora v
        for(auto u : g[v]) {
            // Ako nije uparen, belezimo poslednji sloj
            if(uparivanje[u] == -1 && poslednjiSloj == -1) {
                poslednjiSloj = sloj[v];
            }

            // Ako je uparen i par mu nije obidjen, par je u sledecem sloju
            if(uparivanje[u] != -1 && sloj[uparivanje[u]] == -1) {
                sloj[uparivanje[u]] = sloj[v] + 1;
                red.push(uparivanje[u]);
            }
        }
    }

    return poslednjiSloj;
}

// DFS pretraga koja pronalazi alternirajuci put ukoliko postoji i azurira uparivanje u slojevitom grafu. 
// Vraca da li je alternirajuci put pronadjen.
bool odrediAlternirajuciPut(Cvor v, std::vector<int>& sloj, Uparivanje& uparivanje, int poslednjiSloj, const Graf& g) {
    // Ignorisemo cvorove izvan slojevitog grafa
    if(sloj[v] > poslednjiSloj) {
        return false;
    }

    // Belezimo naredni sloj i "uklanjamo" cvor iz pretrage
    // Uklanjanje se vrsi jer pronadjeni alternirajuci putevi moraju biti
    // disjunktni, ali i kako ne bismo dva puta obilazili iste cvorove u slucaju
    // da alternirajuci put nije pronadjen.
    int sledeciSloj = sloj[v] + 1;
    sloj[v] = -1;

    // Obilazimo sve susede cvora v
    for(auto u : g[v]) {
        // Ako je cvor u neuparen, nasli smo kraj alternirajuceg puta
        if(uparivanje[u] == -1) {
            upari(uparivanje, v, u);
            return true;
        }

        // Ako par cvora u pripada sledecem sloju, nastavljamo pretragu od njega
        if(sloj[uparivanje[u]] == sledeciSloj) {
            // Ako je alternirajuci put pronadjen, azuriramo uparivanje
            if(odrediAlternirajuciPut(uparivanje[u], sloj, uparivanje, poslednjiSloj, g)) {
                upari(uparivanje, v, u);
                return true;
            }
        }
    }

    return false;
}

// Implementira Hopcroft-Karp algoritam za pronalazenje optimlanog uparivanja.
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

struct TezinskaGrana {
    Cvor u, v;
    long long tezina;
};

struct Rezultat {
    long long kriticnaTezina;
    Uparivanje uparivanje;
};

int brojParova(const Uparivanje& uparivanje) {
    int broj = 0;
    for(Cvor u = 0; u < static_cast<Cvor>(uparivanje.size()); ++u) {
        if(uparivanje[u] > u) {
            ++broj;
        }
    }
    return broj;
}

// Formira G_prag od grana tezine <= prag i nalazi optimalno uparivanje u njemu.
Uparivanje uparivanjeDoPraga(int n, const std::vector<TezinskaGrana>& grane, const std::vector<Cvor>& V, long long prag) {
    // ZADATAK: Dopuni samo telo ove funkcije.
    
    // 1. Napravi praznu listu suseda sa n cvorova.
    Graf susedi(n);

    // 2. Prolazi kroz sve tezinske grane. Zadrzi tacno one sa tezinom <= prag.
    // 3. Za svaku zadrzanu neusmerenu granu dodaj oba smera u listu suseda.
    for(const auto& grana: grane) {
        if(grana.tezina <= prag) {
            susedi[grana.u].push_back(grana.v);
            susedi[grana.v].push_back(grana.u);
        }
    }

    // 4. Pozovi datu funkciju optimalnoUparivanje za taj graf i skup V.
    // Vrati njen rezultat; vazan je broj parova, a ne izbor konkretnih grana.
    return optimalnoUparivanje(susedi, V);
    
    // Privremeni povratak omogucava kompajliranje nedovrsenog sablona.
    // return Uparivanje(n, -1);
}

// Najpre maksimizuje broj grana, zatim minimizuje tezinu najteze grane.
Rezultat minimalnaKriticnaTezina(int n, const std::vector<TezinskaGrana>& grane, const std::vector<Cvor>& V) {
    // ZADATAK: Dopuni samo telo ove funkcije.
    // 1. Ako nema grana, vrati prazno uparivanje i 0 kao konvenciju za izlaz.
    // Maksimum tezina praznog skupa nije matematicki definisan u zadatku.
    if(grane.empty()) {
        return {0, Uparivanje(n, -1)};
    }

    // 2. Izdvoj tezine, sortiraj ih i ukloni ponavljanja.
    std::vector<long long> tezine;
    for(const auto& grana: grane) {
        tezine.push_back(grana.tezina);
    }
    std::sort(tezine.begin(), tezine.end());
    tezine.erase(std::unique(tezine.begin(), tezine.end()), tezine.end());

    // 3. Izracunaj cilj K: broj grana optimalnog uparivanja celog grafa.
    // Za to mozes da koristis najveci prag i pomocnu funkciju brojParova.
    int K = brojParova(uparivanjeDoPraga(n, grane, V, tezine.back()));

    // 4. Binarnom pretragom indeksa tezina nadji najmanji dozvoljeni prag:
    // pozovi uparivanjeDoPraga i uporedi broj dobijenih parova sa K.
    // Ako je K dostignuto, odgovor trazi ulevo; inace trazi udesno.
    int l = 0; int d = tezine.size() - 1;
    while(l < d) {
        int s = l + (d-l) / 2;;
        const auto tmp = uparivanjeDoPraga(n, grane, V, tezine[s]);
        if(brojParova(tmp) == K) {
            d = s;
        } else {
            l = s+1;
        }
    }

    // 5. Jos jednom nadji uparivanje za najmanji uspesni prag i vrati oba.
    // Ne smanjuj broj parova da bi dobio jeftiniju granu i ne minimizuj zbir.
    return {tezine[l], uparivanjeDoPraga(n, grane, V, tezine[l])};
    
    // Privremeni povratak omogucava kompajliranje nedovrsenog sablona.
    // return {0, Uparivanje(n, -1)};
}

int main() {
    int n1, n2, m;
    std::cin >> n1 >> n2 >> m;
    std::vector<TezinskaGrana> grane(m);

    for(auto& grana : grane) {
        std::cin >> grana.u >> grana.v >> grana.tezina;
    }

    std::vector<Cvor> V(n1);
    std::iota(V.begin(), V.end(), 0);

    const auto resenje = minimalnaKriticnaTezina(n1 + n2, grane, V);
    std::cout << brojParova(resenje.uparivanje) << " " << resenje.kriticnaTezina << std::endl;
    
    for(Cvor u = 0; u < static_cast<Cvor>(resenje.uparivanje.size()); ++u) {
        if(resenje.uparivanje[u] > u) {
            std::cout << u << " " << resenje.uparivanje[u] << std::endl;
        }
    }

    return 0;
}
