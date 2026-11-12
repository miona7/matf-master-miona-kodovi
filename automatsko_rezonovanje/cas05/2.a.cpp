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

int c(int i) {
    return 18 + i + 1;
}

int main() {
    CNF formula;

    for(int i = 0; i < 5; i++) {
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

    // pocetno stanje (p(0), q(0), r(0)) = (0, 1, 0)
    formula.push_back({-p(0)});
    formula.push_back({q(0)});
    formula.push_back({-r(0)});

    Clause target;
    for(int i = 0; i < 5; i++) {
        // c(i) <=> stanje Si = (1, 1, 1) tj. c(i) <=> p(i) & q(i) & r(i)
        formula.push_back({-c(i), p(i)});
        formula.push_back({-c(i), q(i)});
        formula.push_back({-c(i), r(i)});
        formula.push_back({-p(i), -q(i), -r(i), c(i)});

        target.push_back(c(i));
    }
    formula.push_back(target); 
    // dostize se tokom prvih 5 taktova
    // a najmanji broj taktova koji je potreban je 3 (u petlji menjati vrednost za i)

    std::cout << "p cnf 23 " << formula.size() << std::endl;
    for(const auto c: formula) {
        for(const auto v: c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    } 
}