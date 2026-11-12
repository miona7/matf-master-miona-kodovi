; Petao, Gavran i Kukavica se takmiče u pevanju. U žiriju se nalazi određeni broj sudija. Svaki sudija
; je glasao za jednog od takmičara. Detlić je bio zadužen za brojanje glasova. Ukupno je izbrojao 59
; glasova. Po njegovom brojanju, 15 ih je glasalo za Petla ili Gavrana (u smislu da je zbir njihovih
; glasova toliki), 18 za Gavrana ili Kukavicu, a 20 za Kukavicu ili Petla. Detliću brojanje nije jača
; strana, tako da ni jedan od ova četiri broja ne mora biti tačan, ali se zna da ni u jednom slučaju nije
; pogrešio za više od 13. Koji su rezultati glasanja? Dokazati da je rešenje jedinstveno.

(set-logic QF_LIA)

(declare-const p Int)
(declare-const g Int)
(declare-const k Int)

(assert
    (and
        (<= 46 (+ p g k) 72)
        (<= 2 (+ p g) 28)
        (<= 5 (+ g k) 31)
        (<= 7 (+ k p) 33)
    )
)

(assert
    (or
        (distinct p 15)
        (distinct g 13)
        (distinct k 18)
        (distinct (+ p g k) 46)
    )
)

(check-sat)
(get-value (p g k (+ p g k)))
(exit)