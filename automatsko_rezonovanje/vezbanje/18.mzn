% N-kraljica sa dodatnim uslovom na susedne kolone
% Za dati prirodan broj n >= 4, potrebno je na tablu nxn postaviti n kraljica (po jednu u svaku kolonu) tako da se nijedne dve ne napadaju (isti red ili ista dijagonala),
% uz dodatni uslov da razlika redova kraljica u susednim kolonama bude bar 2: abs(x_{i+1} - x_i) >= 2.
% a) Napisati MiniZinc model, pronaći jedno rešenje za n = 6 i prebrojati sva rešenja za n = 6.
% b) Dodati barem jedno ograničenje za razbijanje simetrija (npr. simetrija po vertikali/horizontali ili uslov na prvi element) i proveriti kako utiče na broj rešenja.

include "globals.mzn";

int: n = 6;

array [1..n] of var 1..n: q; 

constraint all_different(q);

constraint forall(i, j in 1..n where i < j)(
  abs(q[i] - q[j]) != abs(i - j)
);

constraint forall(i in 1..(n-1))(
  abs(q[i+1] - q[i]) >= 2
);
% broj resenja 4

constraint q[1] <= n div 2;
% broj resenja 2

solve satisfy;