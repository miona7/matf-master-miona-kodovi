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

// matrica velicine V * (V+1) gde su redovi cvorovi, a kolone moguce pozicije u putanji
int p(int i, int j, int V) {
    return (i-1) * (V+1) + j;
}

int main() {
    const int V = 5;
    const std::vector<std::pair<int, int>> E = {{1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 1}, {4, 5}, {5, 2}};

    CNF formula;

    // na svakom mestu u putanji mora se naci 1 cvor
    for(int l = 1; l <= V+1; l++) {
        Clause c;
        for(int i = 1; i <= V; i++) {
            c.push_back({p(i, l, V)});
        }
        formula.push_back(c);
    }

    // na svakom mestu u putanji moze se naci najvise 1 cvor
    for(int l = 1; l <= V+1; l++) {
        for(int i = 1; i <= V; i++) {
            for(int j = i+1; j <= V; j++) {
                formula.push_back({-p(i, l, V), -p(j, l, V)});
            }
        }
    }

    // svaki cvor sem prvog moze se naci tacno na 1 mestu u putanji
    for(int i = 2; i <= V; i++) {
        for(int j = 1; j <= V+1; j++) {
            for(int k = j+1; k <= V+1; k++) {
                formula.push_back({-p(i, j, V), -p(i, k, V)});
            }
        }
    } 

    // ako je cvor i na poziciji j, onda na (j-1)-voj poziciji mora da bude cvor d takav da postoji grana (d, i)
    for(int i = 1; i <= V; i++) {
        for(int j = 2; j <= V+1; j++) {
            Clause c = {-p(i, j, V)};

            for(const auto& e : E) {
                if(e.second == i) {
                    c.push_back({p(e.first, j-1, V)});
                }
            }

            formula.push_back(c);
        }
    }

    // na prvom mestu mora da bude cvor 1
    formula.push_back({p(1, 1, V)});

    // na poslednjem mestu mora da bude cvor 1
    formula.push_back({p(1, V+1, V)});

    std::cout << "p cnf " << V * (V+1) << " " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto& v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}