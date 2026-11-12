; Za fragment koda:
; int x, y;
; ...
; if(x > y)
;   max = x;
; else
;   max = y;
; dokazati da je max jednak većoj od promenljivih x i y.

(set-logic QF_BV)

(declare-const x (_ BitVec 32))
(declare-const y (_ BitVec 32))

(define-fun max ((a (_ BitVec 32)) (b (_ BitVec 32))) (_ BitVec 32)
    (ite (bvsgt a b) a b)
)

(assert
    (not
        (and
            (bvsge (max x y) x)
            (bvsge (max x y) y)
            (or
                (= (max x y) x)
                (= (max x y) y)
            )
        )
    )
)

(check-sat)
(exit)