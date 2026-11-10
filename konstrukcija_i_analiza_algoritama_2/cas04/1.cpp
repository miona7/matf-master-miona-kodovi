#include <iostream>
#include <vector>
#include <algorithm>

using Cvor = int;

// Graf cuvamo kao matricu povezanosti
using Graf = std::vector<std::vector<bool>>;

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

// Vraca jedno maksimalno uparivanje u datom grafu
Uparivanje maksimalnoUparivanje(const Graf& graf) {
    Uparivanje uparivanje(graf.size(), -1);

    // Iteriramo kroz sve cvorove grafa
    for(Cvor v = 0; v < graf.size(); v++) {
        // Ako je cvor vec uparen, preskacemo ga
        if(uparivanje[v] != -1) {
            continue;
        }

        // Iteriramo kroz sve njegove susede
        for(Cvor u = 0; u < graf.size(); u++) {
            if(graf[v][u] == false) {
                continue;
            }
          
            // Ako je sused slobodan, uparujemo ga sa trenutnim cvorom
            if(uparivanje[u] == -1) {
                upari(uparivanje, v, u);
                break;
            }
        }
    }

    return uparivanje;
}

// Vraca jedno savrseno uparivanje u gustom grafu (svaki cvor ima barem n/2 suseda, gde je ukupan broj cvorova n)
Uparivanje savrsenoUparivanjeGust(const Graf& graf) {
    // ZADATAK: Dopuni ovu funkciju. Ostale funkcije su vec date.
    
    // 1. Napravi pocetno maksimalno uparivanje pomocu date funkcije.
    Uparivanje uparivanje = maksimalnoUparivanje(graf);

    // 2. U petlji pronadji dva slobodna cvora (njihov par je -1).
    //      Ako ih nema, postupak je zavrsen: vrati dobijeno uparivanje.
    while(true) {
        std::vector<Cvor> slobodni;
        for(Cvor v = 0; v < graf.size() && slobodni.size() < 2; v++) {
            if(uparivanje[v] == -1) {
                slobodni.push_back(v);
            }
        }

        if(slobodni.size() < 2) {
            break;
        }

        // 3. Pregledaj grane tekuceg uparivanja: za cvor u1 procitaj njegov par u2.
        // Preskoci slobodne cvorove. Razmotri oba redosleda krajeva grane.
        Cvor v1 = slobodni[0]; Cvor v2 = slobodni[1];
        for(Cvor u1 = 0; u1 < graf.size(); u1++) {
            Cvor u2 = uparivanje[u1];
            if(u2 == -1) {
                continue;
            }

            // 4. Proveri mozes li krajeve te grane spojiti sa dva razlicita slobodna
            // cvora. Potrebne su dve disjunktne nove grane.
            // 5. Pomocu dva poziva funkcije upari zameni jednu granu dvema novim.
            // Zatim prekini pregled grana za ovaj par slobodnih cvorova.
            if(graf[u1][v2] && graf[u2][v1]) {
                upari(uparivanje, u1, v2);
                upari(uparivanje, u2, v1);
                break;
            }
        }

        // 6. Ponovi postupak dok svi cvorovi ne budu upareni.
    }
    
    
    // PRIVREMENO: Ovaj povratak omogucava kompajliranje, ali ne resava zadatak.
    // Ukloni ga kada napises svoje resenje i vrati izracunato uparivanje.
    // return Uparivanje(graf.size(), -1);
    return uparivanje;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    Graf graf(n, std::vector<bool>(n, false));
    
    while(m--) {
        int u, v;
        std::cin >> u >> v;
        graf[u][v] = graf[v][u] = true;
    }

    Uparivanje u = savrsenoUparivanjeGust(graf);
    std::vector<std::pair<Cvor, Cvor>> izlaz;
    
    for(int i = 0; i < u.size(); i++) {
        if(u[i] != -1 && u[i] > i) {
            izlaz.push_back({i, u[i]});
        }
    }

    std::cout << izlaz.size() << std::endl;
    
    for(auto p : izlaz) {
        std::cout << p.first << " " << p.second << std::endl;
    }

    return 0;
}
