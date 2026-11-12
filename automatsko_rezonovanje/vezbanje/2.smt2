; Pokazati zadovoljivost sledeće formule:
; f(x) = y /\ g(y) = x /\ x != g(f(x))

(set-logic QF_UF)
(declare-sort S 0)

(declare-fun x () S)
(declare-fun y () S)
(declare-fun f (S) S)
(declare-fun g (S) S)

(assert
    (and
        (= (f x) y)
        (= (g y) x)
        (distinct x (g (f x)))
    )
)

(check-sat)
(exit)