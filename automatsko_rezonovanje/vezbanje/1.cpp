// Neka je dat četvorobitni pomerački registar sa linearnom povratnom spregom čiji su bitovi (ui-tom stanju) obelezeni sa 
// pi, qi, ri, si, počev od bita najveće težine. Registar u svakom koraku prelazi iz stanja u stanje na sledeći način:
// pi+1 = ri ^ si (^ je ekskluzivna disjunkcija)
// qi+1 = pi
// ri+1 = qi
// si+1 = ri
// Ako važi da je (p4, q4, r4, s4) = (1, 0, 1, 0), odrediti početno stanje (p0, q0, r0, s0). 
// Zadatak rešiti svođenjem na SAT, a dobijenu iskaznu formulu u CNF-u predstaviti u DIMACS formatu i pokrenuti minisat.

#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i) { return 4*i + 1; }
int q(int i) { return 4*i + 2; }
int r(int i) { return 4*i + 3; }
int s(int i) { return 4*i + 4; }

int main() {
    CNF formula;

    for(int i = 0; i < 4; i++) {
        // p(i+1) = r(i)^s(i)
        formula.push_back({-p(i+1), -r(i), -s(i)});
        formula.push_back({-p(i+1), r(i), s(i)});
        formula.push_back({p(i+1), -r(i), s(i)});
        formula.push_back({p(i+1), r(i), -s(i)});

        // q(i+1) = p(i)
        formula.push_back({p(i), -q(i+1)});
        formula.push_back({-p(i), q(i+1)});

        // r(i+1) = q(i)
        formula.push_back({q(i), -r(i+1)});
        formula.push_back({-q(i), r(i+1)});

        // s(i+1) = r(i)
        formula.push_back({r(i), -s(i+1)});
        formula.push_back({-r(i), s(i+1)});
    }

    // (p4, q4, r4, s4) = (1, 0, 1, 0)
    formula.push_back({p(4)});
    formula.push_back({-q(4)});
    formula.push_back({r(4)});
    formula.push_back({-s(4)});

    // pocetno stanje (p0, q0, r0, s0) = (0, 1, 0, 0)

    std::cout << "p cnf 20 " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}