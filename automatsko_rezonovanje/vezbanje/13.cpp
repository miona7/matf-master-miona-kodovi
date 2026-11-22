// Raspoređivanje ispita po terminima
// Potrebno je rasporediti 6 ispita {I1, I2, I3, I4, I5, I6} u k ispitnih termina {1, 2, ..., k} 
// tako da važe sledeći uslovi:
// 1. Svaki ispit se održava u tačno jednom terminu.
// 2. Neki studenti polažu više ispita, pa sledeći parovi ispita ne smeju biti u istom terminu:
// - {I1, I2}, {I1, I3}, {I2, I4}, {I3, I4}, {I3, I5}, {I4, I6}, {I5, I6}
// 3. Ispit I1 mora da se održi pre ispita I6 (tj. ako je I1 u terminu t1, a I6 u terminu t6, važi t1 < t6). 
// 4. U jednom terminu mogu da se održe najviše dva ispita istovremeno (zbog broja dostupnih sala).
// a) Formulisati SAT kodiranje problema za dato k. Napisati program koji za zadato k generiše DIMACS formulu 
// i pomoću MiniSAT-a pronaći raspored za k = 3 i k = 4.
// b) Uzastopnim pozivima SAT rešavača odrediti najmanji broj termina k za koji ovakav raspored postoji.
// najmanje k za koje raspored postoji je 3

#include <iostream>
#include <vector>

#define BROJ_ISPITA (6)

using Clause = std::vector<int>;
using CNF = std::vector<Clause>;

int p(int i, int j, int k) { return (i-1)*k + j; }

int main() {
    int k;
    std::cin >> k;

    CNF formula;

    // svaki ispit mora da se odrzi bar u jednom terminu
    for(int i = 1; i <= BROJ_ISPITA; i++) {
        Clause c;
        for(int j = 1; j <= k; j++) {
            c.push_back(p(i, j, k));
        }
        formula.push_back(c);
    }

    // svaki ispit se odrzava u najvise jednom terminu
    for(int i = 1; i <= BROJ_ISPITA; i++) {
        for(int j1 = 1; j1 <= k; j1++) {
            for(int j2 = j1 + 1; j2 <= k; j2++) {
                formula.push_back({-p(i, j1, k), -p(i, j2, k)});
            }
        }
    }

    // parovi {I1, I2}, {I1, I3}, {I2, I4}, {I3, I4}, {I3, I5}, {I4, I6}, {I5, I6} ne smeju biti u istom terminu
    for(int j = 1; j <= k; j++) {
        formula.push_back({-p(1, j, k), -p(2, j, k)});
        formula.push_back({-p(1, j, k), -p(3, j, k)});
        formula.push_back({-p(2, j, k), -p(4, j, k)});
        formula.push_back({-p(3, j, k), -p(4, j, k)});
        formula.push_back({-p(3, j, k), -p(5, j, k)});
        formula.push_back({-p(4, j, k), -p(6, j, k)});
        formula.push_back({-p(5, j, k), -p(6, j, k)});
    }

    // ispit I1 mora biti pre ispita I6
    for(int j = 1; j <= k; j++) {
        Clause c;
        c.push_back(-p(1, j, k));
        for(int l = j + 1; l <= k; l++) {
            c.push_back(p(6, l, k));
        }
        formula.push_back(c);
    }
    formula.push_back({-p(6, 1, k)});

    // u svakom terminu moze da se odrzi najvise 2 ispita
    for(int j = 1; j <= k; j++) {
        for(int i1 = 1; i1 <= BROJ_ISPITA; i1++) {
            for(int i2 = i1 + 1; i2 <= BROJ_ISPITA; i2++) {
                for(int i3 = i2 + 1; i3 <= BROJ_ISPITA; i3++) {
                    formula.push_back({-p(i1, j, k), -p(i2, j, k), -p(i3, j, k)});
                }
            }
        }
    }

    std::cout << "p cnf " << BROJ_ISPITA*k << " " << formula.size() << std::endl;
    for(const auto& c : formula) {
        for(const auto& v : c) {
            std::cout << v << " ";
        }
        std::cout << 0 << std::endl;
    }

    return 0;
}