(set-logic QF_LIA)

(declare-const x11 Int)
(declare-const x12 Int)
(declare-const x13 Int)
(declare-const x14 Int)

(declare-const x21 Int)
(declare-const x22 Int)
(declare-const x23 Int)
(declare-const x24 Int)

(declare-const x31 Int)
(declare-const x32 Int)
(declare-const x33 Int)
(declare-const x34 Int)

(declare-const x41 Int)
(declare-const x42 Int)
(declare-const x43 Int)
(declare-const x44 Int)

(assert (<= 1 x11 4))
(assert (<= 1 x12 4))
(assert (<= 1 x13 4))
(assert (<= 1 x14 4))

(assert (<= 1 x21 4))
(assert (<= 1 x22 4))
(assert (<= 1 x23 4))
(assert (<= 1 x24 4))

(assert (<= 1 x31 4))
(assert (<= 1 x32 4))
(assert (<= 1 x33 4))
(assert (<= 1 x34 4))

(assert (<= 1 x41 4))
(assert (<= 1 x42 4))
(assert (<= 1 x43 4))
(assert (<= 1 x44 4))

(assert (distinct x11 x12 x13 x14))
(assert (distinct x21 x22 x23 x24))
(assert (distinct x31 x32 x33 x34))
(assert (distinct x41 x42 x43 x44))

(assert (distinct x11 x21 x31 x41))
(assert (distinct x12 x22 x32 x42))
(assert (distinct x13 x23 x33 x43))
(assert (distinct x14 x24 x34 x44))

(define-fun sum ((a Int) (b Int) (c Int) (d Int)) Int
    (+ 1 
        (ite (< a b) 1 0)
        (ite (and (< a c) (< b c)) 1 0)
        (ite (and (< a d) (< b d) (< c d)) 1 0)
    )
)

(assert
    (= 2 (sum x11 x12 x13 x14))
)

(assert
    (= 3 (sum x21 x22 x23 x24))
)

(assert
    (= 1 (sum x31 x32 x33 x34))
)

(assert
    (= 2 (sum x41 x42 x43 x44))
)

(assert
    (= 3 (sum x14 x13 x12 x11))
)

(assert
    (= 1 (sum x24 x23 x22 x21))
)

(assert
    (= 2 (sum x34 x33 x32 x31))
)

(assert
    (= 2 (sum x44 x43 x42 x41))
)

(assert
    (= 3 (sum x11 x21 x31 x41))
)

(assert
    (= 1 (sum x12 x22 x32 x42))
)    

(assert
    (= 2 (sum x13 x23 x33 x43))
)

(assert
    (= 2 (sum x14 x24 x34 x44))
)

(assert
    (= 2 (sum x41 x31 x21 x11))
)

(assert
    (= 3 (sum x42 x32 x22 x12))
)

(assert
    (= 1 (sum x43 x33 x23 x13))
)

(assert
    (= 3 (sum x44 x34 x24 x14))
)

(assert
    (or
        (distinct x11 1)
        (distinct x12 4)
        (distinct x13 3)
        (distinct x14 2)
        (distinct x21 2)
        (distinct x22 3)
        (distinct x23 1)
        (distinct x24 4)
        (distinct x31 4)
        (distinct x32 1)
        (distinct x33 2)
        (distinct x34 3)
        (distinct x41 3)
        (distinct x42 2)
        (distinct x43 4)
        (distinct x44 1)
    )
)

; resenje je jedinstveno

(check-sat)
(get-value (x11 x12 x13 x14 x21 x22 x23 x24 x31 x32 x33 x34 x41 x42 x43 x44))
(exit)