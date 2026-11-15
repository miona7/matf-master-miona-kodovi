% Costas niz 
% Neka je dat prirodan broj n >= 3. Potrebno je formirati niz (x_1, x_2, ..., x_n) koji predstavlja permutaciju brojeva iz skupa {1, 2, ..., n}, tako da za svako fiksno
% rastojanje d iz skupa {1, ..., n-1} sve razlike x_{i+d} - x_i (za i = 1, ..., n-d) budu međusobno različite.
% Na primer, za n = 4 jedno rešenje je (1, 3, 4, 2), jer su razlike za d = 1: (2, 1, -2), za d = 2: (3, -1), a za d = 3: (1).
% a) Modelovati problem u MiniZinc-u, pronaći jedno rešenje za n = 6, a zatim odrediti ukupan broj svih rešenja za n = 6.
% b) Formulisati ograničenje za razbijanje simetrija kojim se eliminišu trivijalno ekvivalentna rešenja.  

include "globals.mzn";

int: n = 6;

array [1..n] of var 1..n: x;

constraint all_different(x);

constraint forall(d in 1..(n-1))(all_different([x[i+d] - x[i] | i in 1..(n-d)]));

% broj svih resenja je 116

constraint x[1] < x[n];

solve satisfy;
