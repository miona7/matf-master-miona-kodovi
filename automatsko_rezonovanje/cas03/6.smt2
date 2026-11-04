(set-logic QF_NIA)

(declare-const i0 Int)
(declare-const s0 Int)

(declare-const i1 Int)
(declare-const s1 Int)

(declare-const i2 Int)
(declare-const s2 Int)

(declare-const i3 Int)
(declare-const s3 Int)

(declare-const i4 Int)
(declare-const s4 Int)

(declare-const i5 Int)
(declare-const s5 Int)

(declare-const n Int)

(assert (= i0 0))
(assert (= s0 0))
(assert (<= 0 n 5))

(define-fun next_i ((i Int)) Int
    (ite (< i n) (+ i 1) i)
)

(define-fun next_s ((s Int) (i Int)) Int
    (ite (< i n) (+ (next_i i) s) s)
)

(assert (= i1 (next_i i0)))
(assert (= s1 (next_s s0 i0)))

(assert (= i2 (next_i i1)))
(assert (= s2 (next_s s1 i1)))

(assert (= i3 (next_i i2)))
(assert (= s3 (next_s s2 i2)))

(assert (= i4 (next_i i3)))
(assert (= s4 (next_s s3 i3)))

(assert (= i5 (next_i i4)))
(assert (= s5 (next_s s4 i4)))

(assert
    (distinct (* 2 s5) (* n (+ n 1)))
)

(check-sat)
(exit)