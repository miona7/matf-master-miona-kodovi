; Za dati fragment programskog koda:
; double x, y, r;
; ...
; if(x < y)
;   r = y - x;
; else if(y < x)
;   r = x - y;
; else
;   r = 1;
; pokazati da je r različito od nula.

(set-logic QF_LRA)

(declare-const x Real)
(declare-const y Real)

(define-fun f ((a Real) (b Real)) Real
    (ite (< a b) (- b a) (ite (< b a) (- a b) 1))
)

(assert (= (f x y) 0))

(check-sat)
(exit)