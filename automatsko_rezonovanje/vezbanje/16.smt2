; Magični kvadrat 3x3 
; Popuniti matricu 3x3 celim brojevima iz skupa {1, 2, ..., 9} tako da se svaki broj pojavljuje tačno jednom i da je zbir u svakoj vrsti, 
; svakoj koloni i na obe glavne dijagonale jednak 15.
; a) Modelovati problem u SMT-LIB 2 formatu i pronaći jedno rešenje pomoću Z3.
; b) Proveriti da li je rešenje (uz fiksiranu gornju levu ćeliju, npr. 8) jedinstveno, ili uopšteno ispitati koliko različitih rešenja postoji 
; dodavanjem blokirajućih klauzula.

(set-logic QF_LIA)

(declare-const x11 Int)
(declare-const x12 Int)
(declare-const x13 Int)

(declare-const x21 Int)
(declare-const x22 Int)
(declare-const x23 Int)

(declare-const x31 Int)
(declare-const x32 Int)
(declare-const x33 Int)

(assert (<= 1 x11 9))
(assert (<= 1 x12 9))
(assert (<= 1 x13 9))

(assert (<= 1 x21 9))
(assert (<= 1 x22 9))
(assert (<= 1 x23 9))

(assert (<= 1 x31 9))
(assert (<= 1 x32 9))
(assert (<= 1 x33 9))

(assert (distinct x11 x12 x13 x21 x22 x23 x31 x32 x33))

(define-fun sum ((a Int) (b Int) (c Int)) Int
    (+ a b c)
)

(assert (= (sum x11 x12 x13) 15))
(assert (= (sum x21 x22 x23) 15))
(assert (= (sum x31 x32 x33) 15))

(assert (= (sum x11 x21 x31) 15))
(assert (= (sum x12 x22 x32) 15))
(assert (= (sum x13 x23 x33) 15))

(assert (= (sum x11 x22 x33) 15))
(assert (= (sum x31 x22 x13) 15))

(assert (= x11 8))

(assert
    (or
        (distinct x11 8)
        (distinct x12 3)
        (distinct x13 4)
        (distinct x21 1)
        (distinct x22 5)
        (distinct x23 9)
        (distinct x31 6)
        (distinct x32 7)
        (distinct x33 2)
    )
)

(check-sat)
(get-value (x11 x12 x13 x21 x22 x23 x31 x32 x33))
(exit)