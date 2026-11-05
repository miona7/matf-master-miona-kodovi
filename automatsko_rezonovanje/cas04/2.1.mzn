include "globals.mzn";
include "data.dzn";

int: n;

array [1..n] of var 1..n: q; 

constraint all_different(q);

constraint all_different([q[i] - i | i in 1..n]);

constraint all_different([q[i] + i | i in 1..n]);

solve satisfy;

output [ "q = ", show(q), "\n" ];