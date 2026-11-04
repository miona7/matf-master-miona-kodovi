(set-logic QF_LIA)

(declare-const x Int)
(declare-const y Int)
(declare-const z Int)

; navodimo formalne parametre, koji sluze kao dzoker vrednosti u funkciji
; greska bi bila da smo stavili x i y, jer su oni u ovom slucaju globalne konstante, tj neke konkretne vrednosti
; ova fja ne bi bila vise apstraktna tj ne bi radila nad bilo kojim vrednostima, vec samo nad x i y
(define-fun abs_diff ((a Int) (b Int)) Int
    ; (if then else) (uslov) (if grana) (else grana)
    (ite (>= a b) (- a b) (- b a))
) 

; (assert (< (abs_diff x y) 0))
; unsat -> vrednost je uvek >= 0

(assert (distinct (abs_diff x y) (abs_diff y x)))
; unsat -> uvek su jednake

(assert (distinct (= x y) (= (abs_diff x y) 0)))
; unsat -> za x = y fja vraca 0

(check-sat)
(exit)