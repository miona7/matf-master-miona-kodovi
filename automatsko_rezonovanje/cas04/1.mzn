int: U = 76; % konstante deklarisemo tako sto navodimo tip: ime = vrednost 

% promenljive deklarisemo tako sto navodimo var domen promenljive: ime promenljive
var 1..U: p; % broj glasova koje je dobio petao
var 1..U: g; % broj glasova koje je dobio gavran
var 1..U: k; % broj glasova koje je dobila kukavica

% ogranicenja
constraint abs(p + g + k - 61) <= 15; 
constraint abs(p + g - 16) <= 15;
constraint abs(g + k - 18) <= 15;
constraint abs(k + p - 20) <= 15;

% constraint p = g /\ g = k; % unsat

% solve satisfy;
% solve minimize p + g + k;
solve maximize p + g + k;