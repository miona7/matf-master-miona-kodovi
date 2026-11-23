#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

// zamisljamo matricu n * (k+1) gde redovi predstavljaju cvorove, a kolone moguce pozicije u putanji
// za svaki cvor i imamo k+1 mogucnost  
int p(int i, int j, int k) {
    return (i-1) * (k+1) + j;
}

int main() {
    const int V = 6;
    const std::vector<std::pair<int, int>> E = {{1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}, {3, 5}, {4, 5}, {4, 6}, {5, 6}};

    int k;
    std::cin >> k;

    CNF formula;

    // na svakom mestu u putanji mora da se nadje 1 cvor
    for(int l = 1; l <= k+1; l++) {
        Clause c;
        for(int i = 1; i <= V; i++) {
            c.push_back(p(i, l, k));
        }
        formula.push_back(c);
    }

    // svaki cvor moze da se nadje najvise 1 put u putanji
    for(int i = 1; i <= V; i++) {
        for(int j1 = 1; j1 <= k+1; j1++) {
            for(int j2 = j1+1; j2 <= k+1; j2++) {
                formula.push_back({-p(i, j1, k), -p(i, j2, k)});
            }
        }
    }

    // na svakom mestu u putanji moze da se nadje tacno 1 cvor
    for(int l = 1; l <= k+1; l++) {
        for(int i = 1; i <= V; i++) {
            for(int j = i+1; j <= V; j++) {
                formula.push_back({-p(i, l, k), -p(j, l, k)});
            }
        }
    }

    // ako se na j-toj poziciji nalazi cvor i, onda se na (j-1)-voj poziciji mora naci cvor d takav da postoji grana (d, i)
    for(int i = 1; i <= V; i++) {
        for(int j = 2; j <= k+1; j++) {
            Clause c = {-p(i, j, k)};

            for(const auto& e : E) {
                if(e.second == i) {
                    c.push_back(p(e.first, j-1, k));
                }
            }

            formula.push_back(c);
        }
    }

    // cvor 1 se mora naci u putanji na 1 mestu
    formula.push_back({p(1, 1, k)});

    // cvor 6 se mora naci u putanji na (k+1)-vom mestu
    formula.push_back({p(6, k+1, k)});

    // broj promenljivih = velicina matrice
    std::cout << "p cnf " << V * (k+1) << " " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto& v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}