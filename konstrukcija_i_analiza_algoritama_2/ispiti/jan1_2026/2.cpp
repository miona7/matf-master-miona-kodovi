#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int rand2() {
    //DOPUNA 2 poena  // 0 ili 1, fer
}

/*
    rand5(): generišemo broj 0..7 kao:
        x = (((b1)*2 + b2)*2 + b3)
    pa odbacimo 5,6,7.
*/

int rand5() {
    //DOPUNA 4 poena
}

/*
    randM(M): nađemo najmanje k sa 2^k >= M.
    Napravimo k-bitni broj samo preko:
        x = x*2 + rand2()
    i odbacujemo ako je x >= M.
*/

int randM(int M) {
   // DOPUNA 4 poena
}

int main() {
    std::srand((unsigned)std::time(nullptr));

    std::cout << "rand5(): ";
    std::vector<int> c(5);
    
    for(int i = 0; i < 10000; i++) {
        c[rand5()]++;
    }

    for(int i = 0; i < 5; i++) {
        std::cout << c[i] << " ";
    }

    std::cout << "\n";

    int x;
    std::cin >> x;
    std::vector<int> d(x);
    std::cout << "randM(x): ";
    for(int i = 0; i < 10000; i++) {
        d[randM(x)]++;
    }
    
    for(int i = 0; i < x; i++) { 
        std::cout << d[i] << " ";
    }
    
    std::cout << "\n";
}
