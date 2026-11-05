int: n = 10;
int: capacity = 10;

array [1..n] of int: weights = [4, 8, 1, 4, 2, 1, 10, 3, 5, 2];

% bin[i] oznacava u kojoj torbi je predmet i
array [1..n] of var 1..n: bin;

% used[i] - torba i je iskoriscena
array [1..n] of var bool: used;

constraint forall(b in 1..n)(sum(i in 1..n where bin[i] = b)(weights[i]) <= capacity);

constraint forall(i in 1..n)(used[bin[i]] = true);

solve minimize sum(b in 1..n)(bool2int(used[b]));

output [
  "Minimalan broj torbi: ", show(sum(b in 1..n)(bool2int(used[b]))), "\n",
  "Raspored predmeta po torbama: ", show(bin), "\n"
];