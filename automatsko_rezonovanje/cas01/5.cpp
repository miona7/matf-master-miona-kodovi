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

int c(int i) {
    return 15 + i + 1;
}

int main() {
    CNF formula;

    for(int i = 0; i < 4; i++) {
        // p(i+1) <=> -p(i)
        formula.push_back({-p(i), -p(i+1)});
        formula.push_back({p(i), p(i+1)});

        // q(i+1) <=> -(p(i) <=> r(i))
        formula.push_back({-p(i), -q(i+1), r(i)});
        formula.push_back({p(i), -q(i+1), -r(i)});
        formula.push_back({-p(i), q(i+1), -r(i)});
        formula.push_back({p(i), q(i+1), r(i)});
        
        // r(i+1) <=> p(i) | q(i)
        formula.push_back({-p(i), r(i+1)});
        formula.push_back({-q(i), r(i+1)});
        formula.push_back({p(i), q(i), -r(i+1)}); 
    }

    // pocetno stanje
    formula.push_back({-p(0)});
    formula.push_back({-q(0)});
    formula.push_back({-r(0)});

    Clause target;
    for(int i = 0; i < 4; i++) {
        // c(i) <=> stanje Si = (0, 1, 0) tj. c(i) <=> -p(i) & q(i) & -r(i)
        formula.push_back({-p(i), -c(i)});
        formula.push_back({q(i), -c(i)});
        formula.push_back({-r(i), -c(i)});
        formula.push_back({p(i), -q(i), r(i), c(i)});

        target.push_back(c(i));
    }
    // target = {c(0), c(1), c(2), c(3)}
    formula.push_back(target); 
    // barem jedno od stanja c(i) mora biti tacno -> UNSAT nikad nismo usli u trazeno stanje

    std::cout << "p cnf 20 " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto& v: c) {
            std::cout << v << " ";
        }
        std::cout << "0" << std::endl;
    }
}