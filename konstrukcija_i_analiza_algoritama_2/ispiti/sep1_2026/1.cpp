#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include <vector>

struct Node {
    int value;
    int next;               // Indeks sledeceg cvora; -1 znaci kraj.
    int reversedNext = -1;  // Pomocni pokazivac.
};

// Paralelno izvrsava action(i) za i = 0, ..., n-1.
// Svaka nit obradjuje svoj interval indeksa vektora.
// Pretpostavke: 1 <= p <= n i action ne baca izuzetke.
template <typename F>
void parallelFor(int n, int p, F action) {
    std::vector<std::thread> threads;
    threads.reserve(p);

    try {
        for(int t = 0; t < p; ++t) {
            int begin = static_cast<int>(1LL * n * t / p);
            int end = static_cast<int>(1LL * n * (t + 1) / p);

            threads.emplace_back([&, begin, end]() {
                for (int i = begin; i < end; ++i)
                    action(i);
            });
        }
    } catch (...) {
        // Ako pokretanje neke niti ne uspe, sacekati vec pokrenute.
        for(auto& thread : threads) {
            thread.join();
        }
        throw;
    }

    for(auto& thread : threads) {
        thread.join();
    }
}

// Menja samo veze izmedju cvorova i vraca novi pocetak.
// Niti izvan ove funkcije ne smeju istovremeno pristupati listi.
int reverseList(std::vector<Node>& a, int head, int p) {
    if(p <= 0) {
        throw std::invalid_argument("Broj niti mora biti pozitivan.");
    }

    if(a.empty()) {
        if(head != -1) {
            throw std::invalid_argument("Prazna lista mora imati head = -1.");
        }
        return -1;
    }

    if(a.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("Previse cvorova za indekse tipa int.");
    }

    const int n = static_cast<int>(a.size());

    if(head < 0 || head >= n) {
        throw std::invalid_argument("Neispravan indeks pocetka.");
    }

    p = std::min(p, n);
    int newHead = -1;

    //VAS KOD: 
    //Stari pocetak postaje kraj obrnute liste.
    
    
    // FAZA 1: citamo stare veze i formiramo pomocne obrnute veze.
    parallelFor(n, p, [&](int i) {
        // [B] VAS KOD: obrnuta veza i novi pocetak liste.
    });

    // Sve niti prve faze zavrsile su rad.
    // FAZA 2: sada mozemo da zamenimo stare veze obrnutim.
    parallelFor(n, p, [&](int i) {
        // [C] VAS KOD: primeniti pomocnu vezu.
    });

    return newHead;
}

void printList(const std::vector<Node>& a, int head) {
    for(int i = head; i != -1; i = a[i].next) {
        std::cout << a[i].value;

        if(a[i].next != -1) {
            std::cout << " -> ";
        }
    }

    std::cout << '\n';
}

int main() {
    // Elementi nisu smesteni u vektoru redosledom liste.
    // Indeksi:              0        1         2        3        4
    std::vector<Node> a = {{30, 4}, {10, 3}, {50, -1}, {20, 0}, {40, 2}};

    int head = 1;
    // Redosled indeksa liste: 1 -> 3 -> 0 -> 4 -> 2.

    std::cout << "Pre:   ";
    printList(a, head);

    head = reverseList(a, head, 3);

    std::cout << "Posle: ";
    printList(a, head);
}
