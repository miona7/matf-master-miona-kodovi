; (set-logic QF_UF)
(set-logic UF)

; uvodimo novu neinterpretiranu sortu (tip) S (ime) sa arnoscu 0 (broj argumenata)
(declare-sort S 0) 

; deklaracija f-je (u ovom slucaju konstante): ime, lista ulaznih argumenata, povratni tip (sorta)
(declare-fun a () S)
(declare-fun b () S)
(declare-fun c () S)

(declare-fun f (S) S)
(declare-fun g (S) S)

; (assert
;     (and
;         (= a b)
;         (distinct (f a) (f b))
;     )
; )

; (assert
;     (and
;         (= (f a) (f b))
;         (distinct a b)
;     )
; )

; (assert
;     (and
;         (= a b)
;         (distinct (g (f a)) (g (f b)))
;     )
; )

; (assert
;     (and
;         (= (f a) b)
;         (= (f b) c)
;         (= (f c) a)
;         (distinct a b c)
;     )
; )

(assert
    (forall ((x S)(y S))
        (=> (= (f x) (f y)) (= x y))
    )
)

(assert
    (=> (= (f (f a)) (f (f b))) (= a b))
)

(check-sat)
(get-value (a b c f))
(exit)