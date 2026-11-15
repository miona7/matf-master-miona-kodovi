// Hamiltonov ciklus
// Dat je usmereni graf G = (V, E), gde su:
// V = {1, 2, 3, 4, 5}, E = {(1, 2), (1, 3), (2, 3), (2, 4), (3, 4), (3, 5), (4, 1), (4, 5), (5, 2)}.
// Potrebno je pronaći jednostavan zatvoreni obilazak (Hamiltonov ciklus) koji kreće iz čvora 1, 
// obilazi sve čvorove grafa tačno jednom i vraća se nazad u čvor 1
// a) Napisati C++ program koji generiše formulu u DIMACS formatu kojom se proverava da li takav ciklus postoji.
// b) Izmeniti program tako da proverava da li u istom grafu postoji jednostavna putanja sa tačno k = 3 grane koja 
// kreće iz čvora 2, završava se u čvoru 5 i obavezno prolazi kroz čvor 3 (na bilo kojoj međupoziciji).

#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

// matrica velicine V * (k+1) gde su redovi cvorovi, a kolone moguce pozicije u putanji
int p(int i, int j, int k) {
    return (i-1) * (k+1) + j;
}

int main() {
    const int k = 3;
    const int V = 5;
    const std::vector<std::pair<int, int>> E = {{1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 1}, {4, 5}, {5, 2}};

    CNF formula;

    // na svakom mestu u putanji mora se naci 1 cvor
    for(int l = 1; l <= k+1; l++) {
        Clause c;
        for(int i = 1; i <= V; i++) {
            c.push_back({p(i, l, k)});
        }
        formula.push_back(c);
    }

    // na svakom mestu u putanji moze se naci najvise 1 cvor
    for(int l = 1; l <= k+1; l++) {
        for(int i = 1; i <= V; i++) {
            for(int j = i+1; j <= V; j++) {
                formula.push_back({-p(i, l, k), -p(j, l, k)});
            }
        }
    }

    // svaki cvor se moze naci tacno na 1 mestu u putanji
    for(int i = 1; i <= V; i++) {
        for(int j1 = 1; j1 <= k+1; j1++) {
            for(int j2 = j1+1; j2 <= k+1; j2++) {
                formula.push_back({-p(i, j1, k), -p(i, j2, k)});
            }
        }
    } 

    // ako je cvor i na poziciji j, onda na (j-1)-voj poziciji mora da bude cvor d takav da postoji grana (d, i)
    for(int i = 1; i <= V; i++) {
        for(int j = 2; j <= k+1; j++) {
            Clause c = {-p(i, j, k)};

            for(const auto& e : E) {
                if(e.second == i) {
                    c.push_back({p(e.first, j-1, k)});
                }
            }

            formula.push_back(c);
        }
    }

    // na prvom mestu mora da bude cvor 2
    formula.push_back({p(2, 1, k)});

    // na poslednjem mestu mora da bude cvor 5
    formula.push_back({p(5, k+1, k)});

    // putanja mora da prodje kroz cvor 3
    Clause c;
    for(int i = 1; i <= k+1; i++) {
        c.push_back({p(3, i, k)});
    }
    formula.push_back(c);

    std::cout << "p cnf " << V * (k+1) << " " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto& v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}