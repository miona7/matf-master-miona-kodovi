% Raspored parova u turniru / grupama
% Dat je paran broj učesnika n (npr. n = 6) podeljen u n/2 parova tokom k rundi tako da u svakoj rundi svako igra tačno jednom, a da se tokom celog turnira 
% nijedan par igrača ne sastane više od jednom.
% a) Modelovati problem u MiniZinc-u za n = 6 i pronaći validan raspored za što veći broj rundi (npr. k = 5).
% b) Formulisati ograničenje za razbijanje simetrija (na primer, fiksiranje prve runde ili leksikografsko uređenje parova unutar runde).

include "globals.mzn";

int: n = 6;
int: k = 5;

% matrica ciji su redovi runde, a kolone igraci, opp[i, j] = l znaci sa se u i-toj rundi j-ti igrac takmici sa igracem l
array [1..k, 1..n] of var 1..n: opp;

% u svakoj rundi: protivnici su razliciti, niko ne igra sam protiv sebe i parovi su uzajamni
constraint forall(r in 1..k)(
  all_different([opp[r, p] | p in 1..n]) /\
  forall(p in 1..n)(
    opp[r, p] != p /\ opp[r, opp[r, p]] = p 
  )
);

% nijedan par se ne ponavlja, tj. svaki igrac tokom k rundi igra protiv razlicitih protivnika
constraint forall(p in 1..n)(all_different([opp[r, p] | r in 1..k]));

% fiksiranje prve runde na (1, 2), (3, 4), (5, 6)
constraint opp[1, 1] = 2 /\ opp[1, 2] = 1 /\
           opp[1, 3] = 4 /\ opp[1, 4] = 3 /\
           opp[1, 5] = 6 /\ opp[1, 6] = 5;

solve satisfy;
