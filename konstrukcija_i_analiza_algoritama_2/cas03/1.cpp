#include <iostream>
#include <vector>
#include <array>
#include <cstddef>
#include <stdexcept>

class BlumovFilter {
    std::size_t m, k;
    std::vector<bool> bitovi;

    // Tri zasebne nastavne hes funkcije, racunate direktno iz x.
    // hes(x, 0) = h1(x), hes(x, 1) = h2(x), hes(x, 2) = h3(x).
    // h_i(x) = ((a[i] * x + b[i]) % 101) % m
    std::size_t hes(unsigned x, std::size_t i) const {
        constexpr std::array<unsigned, 3> a = {1, 3, 7};
        constexpr std::array<unsigned, 3> b = {0, 1, 3};
        return ((a[i] * x + b[i]) % 101u) % m;
    }

public:
    BlumovFilter(std::size_t brojBitova, std::size_t brojFunkcija)
        : m(brojBitova), k(brojFunkcija) {
        
            if(m < 1 || m > 128 || k < 1 || k > 3) {
            throw std::invalid_argument("Neispravni parametri.");
        }

        // TODO 1: napravi niz od tacno m bitova, svi su 0.
        // throw std::logic_error("TODO 1: dopuni inicijalizaciju.");

        bitovi.assign(m, false);
    }

    void dodaj(unsigned x) {
        // TODO 2: za i = 0, ..., k-1 postavi bitovi[hes(x, i)] na 1.
        // Vec postavljene bitove NE menjaj nazad na 0.
        // throw std::logic_error("TODO 2: dopuni dodavanje.");

        for(std::size_t i = 0; i < k; i++) {
            bitovi[hes(x, i)] = true;
        }
    }

    bool mozdaSadrzi(unsigned x) const {
        // TODO 3: ako je bar jedan od k bitova 0, vrati false.
        // Tek ako su SVI odgovarajuci bitovi 1, vrati true.
        // Ova funkcija ne menja niz bitova.
        // throw std::logic_error("TODO 3: dopuni proveru pripadnosti.");

        for(std::size_t i = 0; i < k; i++) {
            if(!bitovi[hes(x, i)]) {
                return false;
            }
        }   

        return true;
    }

    // Gotova pomocna funkcija, koristi je testni program.
    void ispisi() const {
        if(bitovi.size() != m) {
            throw std::logic_error("Niz bitova nema m elemenata.");
        }

        for(bool bit : bitovi) {
            std::cout << (bit ? '1' : '0');
        }
        
        std::cout << '\n';
    }
};
