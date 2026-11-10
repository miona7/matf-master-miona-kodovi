#include <iostream>
#include <vector>

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

// Azurira uparivanje ako je put pronadjen
// Radimo DFS, krecuci se po uparenim cvorovima, pri cemu su jedini
// neupareni cvorovi na pocetku i na kraju alternirajuceg puta
// Vraca da li je pronadjen alternirajuci put pocevsi od cvora u
// Ova informacija nam je neophodna prilikom pretrage
bool odrediAlternirajuciPut(Cvor u, std::vector<bool>& posecen, Uparivanje& uparivanje, const Graf& graf) {
    // TODO 1: Ako je u vec posecen u ovoj pretrazi, ovaj pokusaj nije uspeo.
    if(posecen[u]) {
        return false;
    }

    // U suprotnom oznaci u kao posecen pre obilaska njegovih suseda.
    posecen[u] = true;

    // TODO 2: Razmotri svakog suseda v cvora u.
    // Ako je v neuparen, pronadjen je kraj puta: pomocu upari promeni par
    // i prijavi uspeh. Promenu uparivanja radi tek kada je put pronadjen.
    for(Cvor v: graf[u]) {
        if(uparivanje[v] == -1) {
            upari(uparivanje, u, v);
            return true;
        }

        // TODO 3: Ako je v uparen, nastavi rekurzivnu pretragu iz njegovog para.
        // Ako taj poziv uspe, upari u i v i prosledi informaciju o uspehu.
        if(odrediAlternirajuciPut(uparivanje[v], posecen, uparivanje, graf)) {
            upari(uparivanje, u, v);
            return true;
        }
    }

    // TODO 4: Ako nijedan sused ne omoguci put, prijavi neuspeh.
    // Privremena povratna vrednost omogucava kompajliranje nedovrsenog koda.
    return false;
}

// Implementira Kuhn-ov algoritam za pronalazenje maksimalnog uparivanja.
// Iz svakog cvora pokrece pretragu za alternirajucim putem.
// Primetimo da je dovoljno pokrenuti pretragu iz svakog cvora samo jednom
// zato sto jednom kada je cvor uparen, ostaje uparen do kraja algoritma.
Uparivanje optimalnoUparivanje(const Graf& graf) {
    // TODO 1: Napravi uparivanje u kome su svi cvorovi u pocetku neupareni.
    Uparivanje uparivanje(graf.size(), -1);
    
    // TODO 2: Obidji cvorove grafa. Pretragu pokreci samo iz trenutno
    // neuparenog cvora; vec upareni cvor ne treba novi par.
    for(Cvor v = 0; v < graf.size(); v++) {
        if(uparivanje[v] == -1) {
            // TODO 3: Za SVAKI novi pocetak pretrage napravi nov niz posecenosti,
            // u kome na pocetku nijedan cvor nije posecen.
            std::vector<bool> posecen(graf.size(), false);

            // TODO 4: Pozovi odrediAlternirajuciPut da pokusa da poveca uparivanje.
            // Funkcija sama menja uparivanje kada pronadje odgovarajuci put.
            odrediAlternirajuciPut(v, posecen, uparivanje, graf);
        }
    }
    
    // TODO 5: Posle svih pokusaja vrati dobijeno uparivanje.
    return uparivanje;

    // Privremeni prazan rezultat zameni svojim resenjem ove funkcije.
    // return Uparivanje(graf.size(), -1);
}

int main() {
    int n1, n2, m;
    std::cin >> n1 >> n2 >> m;
    Graf g(n1 + n2);
    
    while(m--) {
        int a, b;
        std::cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    auto u = optimalnoUparivanje(g);
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
