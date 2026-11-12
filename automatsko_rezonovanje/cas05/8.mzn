int: n = 7;
int: m = (n*(n-1)) div 6;

array [1..m, 1..3] of var 1..n: T;

constraint forall(i in 1..m)(
    T[i, 1] < T[i, 2] /\ T[i, 2] < T[i, 3]
);

% svaki par različitih elemenata (x, y) se pojavljuje u tačno jednoj trojci
% pošto su trojke već strogo rastuće, dovoljno je proveriti da li se x i y istovremeno nalaze u redu i, bez obzira na poziciju
constraint forall(x, y in 1..n where x < y)(
  sum(i in 1..m)(
    bool2int(
      (T[i, 1] == x \/ T[i, 2] == x \/ T[i, 3] == x) /\
      (T[i, 1] == y \/ T[i, 2] == y \/ T[i, 3] == y)
    )
  ) = 1
);

constraint T[1, 1] = 1;

solve satisfy;