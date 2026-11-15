; Zagonetka Futoshiki 4x4
; Popuniti tablu 4x4 brojevima iz skupa {1, 2, 3, 4} tako da se u svakoj vrsti i svakoj koloni 
; svaki broj pojavljuje tačno jednom. Pored toga, zadati su sledeći uslovi nejednakosti između
; susednih polja (gde x_{i,j} označava polje u i-toj vrsti i j-toj koloni):  
; U 1. vrsti: x_{1,1} > x_{1,2} i x_{1,3} < x_{1,4}
; U 2. vrsti: x_{2,2} > x_{2,3}
; Između 2. i 3. vrste: x_{2,1} < x_{3,1} i x_{2,4} < x_{3,4}
; U 4. vrsti: x_{4,1} < x_{4,2} i x_{4,3} > x_{4,4}
; Zadate vrednosti: x_{1,3} = 1, x_{3,2} = 1.
; a) Modelovati zagonetku u SMT-LIB 2 formatu i pomoću Z3 pronaći rešenje.
; b) Proveriti da li je rešenje jedinstveno.   

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

(assert (> x11 x12))
(assert (< x13 x14))

(assert (> x22 x23))

(assert (< x21 x31))
(assert (< x24 x34))

(assert (< x41 x42))
(assert (> x43 x44))

(assert (= x13 1))
(assert (= x32 1))

(assert
    (or 
        (distinct x11 4)
        (distinct x12 2)
        (distinct x13 1)
        (distinct x14 3)
        (distinct x21 2)
        (distinct x22 4)
        (distinct x23 3)
        (distinct x24 1)
        (distinct x31 3)
        (distinct x32 1)
        (distinct x33 2)
        (distinct x34 4)
        (distinct x41 1)
        (distinct x42 3)
        (distinct x43 4)
        (distinct x44 2)
    )
)

; nije jedinstveno

(check-sat)
(get-value (x11 x12 x13 x14 x21 x22 x23 x24 x31 x32 x33 x34 x41 x42 x43 x44))
(exit)