int: num_players = 16;
int: num_groups = 4;
int: group_size = 4;
int: w = 5;

% player_group[week, player] - u kojoj grupi je igrac player u nedelji week 
array [1..w, 1..num_players] of var 1..num_groups: player_group;

% svaka grupa mora imati tacno 4 igraca svake nedelje
constraint forall(week in 1..w, g in 1..num_groups)(
  let {
    array [1..num_players] of var 0..1: in_group = [bool2int(player_group[week, p] = g) | p in 1..num_players] 
  } in sum(in_group) = group_size
);

% nijedan par ne sme biti u istoj grupi vise od jednom
constraint forall(p1, p2 in 1..num_players where p1 < p2)(
  sum(week in 1..w)(bool2int(player_group[week, p1] = player_group[week, p2])) <= 1
);

solve satisfy;

output [
  "Nedelja " ++ show(week) ++ ": " ++ show([player_group[week, p] | p in 1..num_players]) ++ "\n" | week in 1..w
];