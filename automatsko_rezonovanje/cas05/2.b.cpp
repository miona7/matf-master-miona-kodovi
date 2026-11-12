#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i) {
    return i*3 + 1;
}

int q(int i) {
    return i*3 + 2;
}

int r(int i) {
    return i*3 + 3;
}

int main() {
    CNF formula;

    for(int i = 0; i < 3; i++) {
        // p(i+1) = ~q(i)
        formula.push_back({-p(i+1), -q(i)});
        formula.push_back({q(i), p(i+1)});

        // q(i+1) = ~(p(i) <=> r(i))
        formula.push_back({-q(i+1), -p(i), -r(i)});
        formula.push_back({-q(i+1), r(i), p(i)});
        formula.push_back({-p(i), r(i), q(i+1)});
        formula.push_back({p(i), -r(i), q(i+1)});

        // r(i+1) = q(i) | r(i)
        formula.push_back({-r(i+1), q(i), r(i)});
        formula.push_back({-q(i), r(i+1)});
        formula.push_back({-r(i), r(i+1)});
    }

    // za pocetno stanje imamo 8 mogucnosti -> isprobamo svaku da vidimo da li se vracamo u to isto stanje
    // iz stanja (0, 1, 1) i (1, 0, 1) se vracamo u isto stanje
    formula.push_back({p(0)});
    formula.push_back({q(0)});
    formula.push_back({r(0)});

    // stanje nakon 3 takta treba da bude ~(pocetno stanje)
    formula.push_back({-p(3), -q(3), -r(3)});

    std::cout << "p cnf 12 " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto v: c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    } 
}