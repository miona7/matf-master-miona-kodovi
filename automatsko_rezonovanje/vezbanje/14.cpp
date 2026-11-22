// Registarsko kolo
// Posmatra se kolo sa tri jednobitna registra a, b, c čije se vrednosti u svakom narednom taktu menjaju po pravilima:
// - a{i+1} = bi
// - b{i+1} = ci
// - c{i+1} = ai ^ bi
// Početno stanje je (a0, b0, c0) = (1, 0, 0).
// a) Korišćenjem SAT rešavača proveriti da li kolo u nekom od prvih 6 taktova dostiže stanje (1, 1, 1) i odrediti 
// najmanji takav korak. -> najmanji korak je 5
// b) Ispitati postoji li neko početno stanje (različito od (0, 0, 0)) koje se posle tačno 4 takta vraća u isto stanje.
// ne postoji

#include <iostream>
#include <vector>

#define BROJ_TAKTOVA (6)

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int a(int i) { return 3*i + 1; }
int b(int i) { return 3*i + 2; }
int c(int i) { return 3*i + 3; }
int t(int i) { return 21 + i + 1; }

int main() {
    CNF formula;

    for(int i = 0; i < BROJ_TAKTOVA; i++) {
        // a{i+1} = bi
        formula.push_back({-a(i+1), b(i)});
        formula.push_back({a(i+1), -b(i)});

        // b{i+1} = ci
        formula.push_back({-b(i+1), c(i)});
        formula.push_back({b(i+1), -c(i)});

        // c{i+1} = ai ^ bi
        formula.push_back({-a(i), b(i), c(i+1)});
        formula.push_back({a(i), -b(i), c(i+1)});
        formula.push_back({a(i), b(i), -c(i+1)});
        formula.push_back({-a(i), -b(i), -c(i+1)});
    }

    // a)
    // pocetno stanje
    // formula.push_back({a(0)});
    // formula.push_back({-b(0)});
    // formula.push_back({-c(0)});

    // Clause target;
    // for(int i = 0; i <= BROJ_TAKTOVA; i++) {
    //     formula.push_back({a(i), -t(i)});
    //     formula.push_back({b(i), -t(i)});
    //     formula.push_back({c(i), -t(i)});
    //     formula.push_back({-a(i), -b(i), -c(i), t(i)});

    //     target.push_back(t(i));
    // }
    // formula.push_back(target);

    // b)
    // formula.push_back({-a(0), a(4)});
    // formula.push_back({a(0), -a(4)});

    // formula.push_back({-b(0), b(4)});
    // formula.push_back({b(0), -b(4)});

    // formula.push_back({-c(0), c(4)});
    // formula.push_back({c(0), -c(4)});

    // zabranjujemo (0, 0, 0)
    // formula.push_back({a(0), b(0), c(0)});

    std::cout << "p cnf 28 " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto& v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }

    return 0;
}