#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int rand100() { // vraca slucajan broj od 1 do 100
	return rand() % 100 + 1;
}

// (a) generisanje x,y,z sa zadatim verovatnocama (5 poena)
int random_xyz(int x, int y, int z, int px, int py, int pz) {
    // DOPUNA
}


// (b) opsti slucaj za n vrednosti (5 poena)
int random_n(const vector<int>& values, const vector<int>& p) {
    // DOPUNA
}

int main() {
    srand(time(NULL));

    cout << "Primer (a):\n";

    for(int i = 0; i < 10; i++) {
        cout << random_xyz(1, 2, 3, 20, 30, 50) << " ";
    }

    cout << "\n\nPrimer (b):\n";

    vector<int> values = {10,20,30,40};
    vector<int> prob   = {10,20,30,40};

    for(int i = 0; i < 10; i++) {
        cout << random_n(values, prob) << " ";
    }

    cout << endl;

    return 0;
}
