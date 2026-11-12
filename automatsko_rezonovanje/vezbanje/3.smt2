; Rešiti sistem jednačina ako se zna da su x i y celi brojevi:
; 2x + 3y = 7
; 3x - 6y = 0

(set-logic QF_LIA)

(declare-const x Int)
(declare-const y Int)

(assert
    (and
        (= (+ (* 2 x) (* 3 y)) 7)
        (= (- (* 3 x) (* 6 y)) 0)
    )
)

(check-sat)
(get-value (x y))
(exit)