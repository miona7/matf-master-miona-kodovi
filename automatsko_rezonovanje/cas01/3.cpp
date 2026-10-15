#include <iostream>
#include <vector>

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i, int j) {
    return (i-1)*4 + j;
}

int main() {
    CNF formula;

    // svako predavanje se mora odrzati u nekom terminu
    for(int i = 1; i <= 4; i++) {
        Clause c;
        for(int j = 1; j <= 4; j++) {
            c.push_back(p(i, j));
        }
        formula.push_back(c);
    }

    for(int i = 1; i <= 4; i++) {
        for(int j = 1; j <= 4; j++) {
            for(int k = j+1; k <= 4; k++) {
                // 2 predavanja ne smeju biti u istom terminu
                formula.push_back({-p(j, i), -p(k, i)});
                // 1 predavanje ne moze biti u 2 termina
                formula.push_back({-p(i, j), -p(i, k)});
            }
        }
    }

    // L-1 A-2 VI-3 BP-4

    // L pre A
    formula.push_back({-p(2, 4), p(1, 1), p(1, 2), p(1, 3)});
    formula.push_back({-p(2, 3), p(1, 1), p(1, 2)});
    formula.push_back({-p(2, 2), p(1, 1)});
    formula.push_back({-p(2, 1)});

    // VI ni u prvom ni u poslednjem terminu
    formula.push_back({-p(3, 1)});
    formula.push_back({-p(3, 4)});
    
    // BP neposredno nakon L ili VI
    formula.push_back({-p(4, 4), p(1, 3), p(3, 3)});
    formula.push_back({-p(4, 3), p(1, 2), p(3, 2)});
    formula.push_back({-p(4, 2), p(1, 1), p(3, 1)});
    formula.push_back({-p(4, 1)});

    // A nije neposredno posle BP
    formula.push_back({-p(2, 4), -p(4, 3)});
    formula.push_back({-p(2, 3), -p(4, 2)});
    formula.push_back({-p(2, 2), -p(4, 1)});

    // L nije u poslednjem terminu
    formula.push_back({-p(1, 4)});

    // blokiranje resenja
    // formula.push_back({-p(1, 1), -p(2, 4), -p(3, 3), -p(4, 2)});
    // formula.push_back({-p(1, 1), -p(2, 2), -p(3, 3), -p(4, 4)});

    // L nije u prvom terminu
    // formula.push_back({-p(1, 1)});

    std::cout << "p cnf 16 " << formula.size() << std::endl;
    for(const auto& c: formula) {
        for(const auto& v: c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }
}