% Magični trougao
% Potrebno je rasporediti sve brojeve iz skupa {1, 2, ..., 9} na stranice jednakostraničnog trougla (po 4 broja na svakoj od 3 stranice, pri čemu se brojevi u temenima dele
% između dve susedne stranice) tako da je zbir brojeva na svakoj od tri stranice jednak istom broju S.   
% a) Napisati MiniZinc model koji pronalazi jedno rešenje za zbir S = 20.
% b) Formulisati ograničenja za razbijanje simetrija (rotacije i refleksije trougla) i odrediti koliko suštinski različitih rešenja postoji za S = 20.   

include "globals.mzn";

int: S = 20;

array [1..3, 1..4] of var 1..9: T;

% Stranica 1: T[1, 1], T[1, 2], T[1, 3], T[1, 4] (temena su T[1, 1] i T[1, 4])
% Stranica 2: T[2, 1], T[2, 2], T[2, 3], T[2, 4] (temena su T[2, 1] i T[2, 4], pri cemu je T[2, 1] = T[1, 4])
% Stranica 3: T[3, 1], T[3, 2], T[3, 3], T[3, 4] (temena su T[3, 1] i T[3, 4], gde je T[3, 1] = T[2, 4], a T[3, 4] = T[1, 1])

constraint T[1, 4] = T[2, 1];
constraint T[3, 1] = T[2, 4];
constraint T[1, 1] = T[3, 4]; 

constraint all_different([T[1, 1], T[1, 2], T[1, 3], T[1, 4], T[2, 2], T[2, 3], T[2, 4], T[3, 2], T[3, 3]]);

constraint forall(i in 1..3)(
    sum(j in 1..4)(T[i, j]) = S 
);

% razbijanje rotacije
constraint T[1, 1] < T[1, 4];
constraint T[1, 1] < T[3, 1];

% razbijanje refleksije
constraint T[2, 1] < T[3, 1];

% razbijanje simetrije na svakoj od stranica
constraint forall(i in 1..3)(
  T[i, 2] < T[i, 3]
);

solve satisfy;