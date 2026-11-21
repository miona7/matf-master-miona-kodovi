/* Zadatak 1 (Blumov filter – Brojanje procenjenih elemenata i presek filtera)

Proširi klasu BlumovFilter sa dve nove metode:

- std::size_t brojPostavljenihBitova() const — vraća ukupan broj bitova koji su postavljeni na 1 u unutrašnjem vektoru bitovi.

- bool presek(const BlumovFilter& drugi) const — prima referencu na drugi Blumov filter (pretpostavlja se da imaju 
iste parametre m i k) i vraća true ako je presek njihovih bitova neprazan (tj. ako postoji bar jedna pozicija na kojoj 
su oba filtera postavila bit na 1). U suprotnom vraća false. */

#include <iostream>
#include <vector>
#include <array>
#include <cstddef>
#include <stdexcept>
#include <cassert>

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
            
            bitovi.assign(m, false);
    }

    void dodaj(unsigned x) {
        for(std::size_t i = 0; i < k; i++) {
            bitovi[hes(x, i)] = true;
        }
    }

    bool mozdaSadrzi(unsigned x) const {
        for(std::size_t i = 0; i < k; i++) {
            if(!bitovi[hes(x, i)]) {
                return false;
            }
        }
        return true;
    }

    std::size_t brojPostavljenihBitova() const {
        std::size_t rez = 0;
        for(std::size_t i = 0; i < bitovi.size(); i++) {
            if(bitovi[i]) {
                rez++;
            }
        }
        return rez;
    }

    bool presek(const BlumovFilter& drugi) const {
        for(std::size_t i = 0; i < bitovi.size(); i++) {
            if(bitovi[i] && drugi.bitovi[i]) {
                return true;
            }
        }
        return false;
    }

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

int main() {
    // Kreiramo dva filtera sa m = 32 bita i k = 3 heš funkcije
    BlumovFilter bf1(32, 3);
    BlumovFilter bf2(32, 3);

    // 1. Test početnog stanja
    assert(bf1.brojPostavljenihBitova() == 0);
    assert(bf1.mozdaSadrzi(10) == false); // Sigurno nije u filteru

    // 2. Dodavanje elemenata u prvi filter
    bf1.dodaj(10);
    bf1.dodaj(20);
    bf1.dodaj(30);

    std::size_t postavljenih1 = bf1.brojPostavljenihBitova();
    assert(postavljenih1 > 0);

    // Provera pripadnosti za bf1
    assert(bf1.mozdaSadrzi(10) == true);
    assert(bf1.mozdaSadrzi(20) == true);
    assert(bf1.mozdaSadrzi(30) == true);

    // 3. Dodavanje elemenata u drugi filter (neki se preklapaju, neki ne)
    bf2.dodaj(30);
    bf2.dodaj(50);

    assert(bf2.mozdaSadrzi(30) == true);
    assert(bf2.mozdaSadrzi(50) == true);
    assert(bf2.mozdaSadrzi(10) == false); // bf2 nema 10 (mada može biti lažno pozitivan, ali za ove parametre uglavnom vrati false)

    // 4. Test preseka filtera
    // Pošto oba filtera sadrže ključ 30, njihovi bitovi se moraju preklapati
    assert(bf1.presek(bf2) == true);

    // Kreiramo treći potpuno prazan filter
    BlumovFilter bf3(32, 3);
    assert(bf1.presek(bf3) == false); // Prazan filter nema postavljenih bitova u preseku sa bf1

    std::cout << "Svi test primeri za Blumov filter su uspesno prosli!\n";
    return 0;
}
