; Rešiti sistem nejednačina ako su x i y realni brojevi:
; 2x + 3y > 1
; 3x - 2y < 0

(set-logic QF_LRA)

(declare-const x Real)
(declare-const y Real)

(assert
    (and
        (> (+ (* 2 x) (* 3 y)) 1)
        (< (- (* 3 x) (* 2 y)) 0)
    )
)

(check-sat)
(get-value (x y))
(exit)