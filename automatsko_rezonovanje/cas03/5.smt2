(set-logic QF_BV)

; deklarisemo konstante koje su tipa BitVec sirine 8 bita, _ je operator za parametrizaciju
(declare-const x (_ BitVec 8))
(declare-const y (_ BitVec 8))

; za bitvektore imamo specijalne operatore za sabiranje, mnozenje, ...
; bitno je da se naglasi da li se radi sa oznacenim ili neoznacenim brojevima
(define-fun is_greater ((a (_ BitVec 8)) (b (_ BitVec 8))) Bool
    (bvuge (bvadd a b) a)
)

(assert (distinct (is_greater x y) true))
; sat -> x = #x01 y = #xff

(define-fun overflow ((a (_ BitVec 8)) (b (_ BitVec 8))) Bool
    (bvult (bvsub (_ bv255 8) a) b)
)

(assert
    (= (is_greater x y) (overflow x y))
)

(check-sat)
(get-value (x y))
(exit)