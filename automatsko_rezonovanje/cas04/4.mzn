include "globals.mzn";

int: k = 5;
% int: n = 6;

% array [1..k] of var 0..n: x;

array [1..k] of var 0..(2^(k-1) - 1): x;

constraint all_different(x);

% constraint x[1] = 0 /\ x[k] = n;

constraint x[1] = 0;

constraint forall(i in 1..k-1)(x[i+1] - x[i] > 0);

constraint all_different([x[j] - x[i] | i, j in 1..k where i < j]);

% solve satisfy;

solve minimize x[k] - x[1];