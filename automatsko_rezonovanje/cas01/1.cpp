#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int main() {
    // a - Ana je vitez, b - Boris je vitez, c - Cane je vitez
    int a = 1; int b = 2; int c = 3;

    CNF formula;

    // Ana: Boris je lazov
    formula.push_back({-a, -b});
    formula.push_back({a, b});

    // Boris: Ana i Cane su iste vrste
    formula.push_back({-a, -b, c});
    formula.push_back({a, -b, -c});
    formula.push_back({-a, b, -c});
    formula.push_back({a, b, c});

    // Cane: Ana je lazov
    formula.push_back({-a, -c});
    formula.push_back({a, c});

    std::cout << "p cnf 3 " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto& v: c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}