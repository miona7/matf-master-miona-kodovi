; (set-logic QF_LRA)
(set-logic QF_LIA)

; (declare-const x Real)
; (declare-const y Real)
; (declare-const z Real)

(declare-const x Int)
(declare-const y Int)
(declare-const z Int)

(assert
    (= (+ (* 2 x) (* 3 y) (* (- 1) z)) 7)
)

(assert
    (>= (+ x (* (- 1) y) (* 2 z)) 7)
)

(assert
    (>= (+ (* 3 x) (* 2 y) z) 10)
)

(assert
    (and
        (>= 10 x 0)
        (>= 10 y 0)
        (>= 10 z 0)
    )
)

; (assert
;     (<= (+ x y z) 6)
; )

(minimize (+ x y z))

(check-sat)
(get-value (x y z))
(exit)