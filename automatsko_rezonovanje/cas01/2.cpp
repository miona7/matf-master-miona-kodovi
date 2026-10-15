#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i, int j, int k) {
    return (i-1)*k + j;
}

int main() {
    // broj cvorova, broj grana, broj boja
    int m, n, k;
    std::cin >> m >> n >> k;
    
    CNF formula;
    
    // svaki cvor mora biti obojen barem u jednu boju
    for(int u = 1; u <= m; u++) {
        Clause c;
        for(int i = 1; i <= k; i++) {
            c.push_back(p(u, i, k));
        }
        formula.push_back(c);
    }

    // svaki cvor sme biti obojen u najvise jednu boju
    for(int u = 1; u <= m; u++) {
        for(int i = 1; i <= k; i++) {
            for(int j = i+1; j <= k; j++) {
                formula.push_back({-p(u, i, k), -p(u, j, k)});
            }
        }
    }

    // susedni cvorovi ne smeju biti obojeni u istu boju
    for(int i = 0; i < n; i++) {
        int u, v;
        std::cin >> u >> v;

        for(int j = 1; j <= k; j++) {
            formula.push_back({-p(u, j, k), -p(v, j, k)});
        }
    }

    std::cout << "p cnf " << m*k << " " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto& v: c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}