include "globals.mzn";

int: n = 4;

% matrice deklarisemo kao niz sa 2 dimenzije
array [1..n, 1..n] of var 1..16: grid;

constraint all_different([grid[i, j] | i in 1..n, j in 1..n]);

% forall(iterator)(sta treba da vazi)
% sum(iterator)(sta se sumira)
constraint forall(i in 1..n)(sum(j in 1..n)(grid[i, j]) = 34); % zbirovi vrsta = 34
constraint forall(j in 1..n)(sum(i in 1..n)(grid[i, j]) = 34); % zbirovi kolona = 34 
constraint sum(i in 1..n)(grid[i, i]) = 34; % zbirovi na glavnoj dijagonali = 34
constraint sum(i in 1..n)(grid[i, n-i+1]) = 34; % zbirovi na sporednoj dijagonali = 34 

% constraint grid[1, 2] = 12 /\ grid[2, 2] = 8 /\ grid[2, 3] = 15 /\ grid[3, 1] = 7 /\ grid[3, 3] = 2 /\ grid[4, 1] = 4 /\ grid[4, 4] = 11;

constraint grid[1, 1] < grid[1, n] /\ grid[1, 1] < grid[n, 1] /\ grid[1, 1] < grid[n, n];
constraint grid[1, n] < grid[n, 1];

solve satisfy;