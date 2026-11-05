include "globals.mzn";

int: n = 10;
int: capacity = 10;

array [1..n] of int: weight = [4, 8, 1, 4, 2, 1, 10, 3, 5, 2];
array [1..n] of var 1..n: bin;
array [1..n] of var 0..capacity: load;

constraint bin_packing_load(load, bin, weight);

solve minimize sum(b in 1..n)(bool2int(load[b] > 0));

output [
  "Minimalan broj torbi: ", show(sum(b in 1..n)(bool2int(load[b] > 0))), "\n",
  "Opterećenje torbi: ", show(load), "\n",
  "Raspored predmeta po torbama: ", show(bin), "\n"
];