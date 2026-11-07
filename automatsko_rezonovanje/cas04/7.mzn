int: num_players = 16;
int: num_groups = 4;
int: group_size = 4;
int: w = 5;

% player_group[week, player] - u kojoj grupi je igrac player u nedelji week 
array [1..w, 1..num_players] of var 1..num_groups: player_group;

% svaka grupa mora imati tacno 4 igraca svake nedelje
constraint forall(week in 1..w, g in 1..num_groups)(
  sum(p in 1..num_players)(bool2int(player_group[week, p] = g)) = group_size
);

% nijedan par ne sme biti u istoj grupi vise od jednom
constraint forall(p1, p2 in 1..num_players where p1 < p2)(
  sum(week in 1..w)(bool2int(player_group[week, p1] = player_group[week, p2])) <= 1
);

% ogranicenje fiksira raspored u prvoj nedelji: igrači 1–4 su u grupi 1, 5–8 u grupi 2 itd
constraint forall(p in 1..num_players)(
  player_group[1, p] = (p-1) div group_size + 1
);

% zahtevamo da igrač 1 u svakoj narednoj nedelji pripada grupi označenoj brojem 1
constraint forall(week in 1..w)(player_group[week, 1] = 1);

solve satisfy;

output [
  "Nedelja " ++ show(week) ++ ": " ++ show([player_group[week, p] | p in 1..num_players]) ++ "\n" | week in 1..w
];