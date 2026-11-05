include "globals.mzn";

int: n = 4;

array [1..n] of var 1..2*n: first; % za svaki element od 1 do n cuva prvo pojavljivanje tog elementa
array [1..n] of var 1..2*n: second; % za svaki element od 1 do n cuva drugo pojavljivanje tog elementa

constraint all_different(first ++ second);

constraint forall(i in 1..n)(second[i] - first[i] = i + 1);

array [1..2*n] of var 1..n: res;

constraint forall(i in 1..n)(
  res[first[i]] = i /\ res[second[i]] = i
);

solve satisfy;