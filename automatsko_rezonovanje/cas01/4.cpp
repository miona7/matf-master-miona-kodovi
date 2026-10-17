#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i) {
    return 3*i + 1;
}

int q(int i) {
    return 3*i + 2;
}

int r(int i) {
    return 3*i + 3;
}

int main() {
    CNF formula;

    for(int i = 0; i < 4; i++) {
        // p(i+1) <=> -p(i)
        formula.push_back({-p(i), -p(i+1)});
        formula.push_back({p(i), p(i+1)});

        // q(i+1) <=> -(p(i) <=> q(i))
        formula.push_back({-p(i), -q(i), -q(i+1)});
        formula.push_back({p(i), q(i), -q(i+1)});
        formula.push_back({-p(i), q(i), q(i+1)});
        formula.push_back({p(i), -q(i), q(i+1)});
        
        // r(i+1) <=> p(i) & q(i)
        formula.push_back({p(i), -r(i+1)});
        formula.push_back({q(i), -r(i+1)});
        formula.push_back({-p(i), -q(i), r(i+1)}); 
    }

    // pocetno stanje
    formula.push_back({-p(0)});
    formula.push_back({q(0)});
    formula.push_back({-r(0)});

    // stanje -S4 -> ocekujemo UNSAT ukoliko se kolo vraca u pocetno stanje
    formula.push_back({p(4), -q(4), r(4)});

    std::cout << "p cnf 15 " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto& v: c) {
            std::cout << v << " ";
        }
        std::cout << "0" << std::endl;
    }
}