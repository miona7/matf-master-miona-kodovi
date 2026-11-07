include "data.dzn";

int: n;

% niz deklarisemo sa array[dim] of var vrednosti koje promenljive mogu da uzimaju
array [1..n] of var 1..n: q; % q[i] = j -> dama u i-toj koloni se nalazi u vrsti j -> nemamo dve dame u istoj koloni

% dve dame nisu u istom redu
constraint forall(i, j in 1..n where i < j)(q[i] != q[j]);

% dve dame nisu na istoj dijagonali
constraint forall(i, j in 1..n where i < j)(
  q[i] != q[j] /\ q[i] - i != q[j] - j /\ q[i] + i != q[j] + j);

solve satisfy;

% za ispis koristimo output [string]
output [ "q = ", show(q), "\n" ];