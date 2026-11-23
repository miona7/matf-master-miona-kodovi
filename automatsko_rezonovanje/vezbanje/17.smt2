; Posmatra se funkcija nad 8-bitnim označenim celim brojevima:
; int8_t clamp_abs_diff(int8_t a, int8_t b) {
;     int8_t diff = a - b;
;     if (diff < 0) diff = -diff;
;     if (diff > 10) diff = 10;
;     return diff;
; }
; Uz pretpostavku da su ulazi ograničeni na označene 8-bitne vrednosti u opsegu [-60, 60]:
; a) Predstaviti stanja programa i relacije prelaza u SMT-LIB 2 formatu koristeći (_ BitVec 8).
; b) Pomoću Z3 dokazati da za sve dozvoljene ulaze važi označeno ograničenje 0 <= clamp_abs_diff(a, b) <= 10
; c) Proveriti da li za sve dozvoljene ulaze važi: ako je clamp_abs_diff(a, b) = 0, onda je a = b.
; d) (Bonus provera bit-vektora): Šta se dešava ako se ograničenje ulaza [-60, 60] ukloni i dozvole svi 8-bitni označeni 
; brojevi (int8_t, tj. [-128, 127])? Da li svojstvo iz pod b) i dalje važi ili dolazi do overflow-a kod bvneg 
; (npr. za diff = -128, odnosno #x80)? Ako postoji kontraprimer, ispisati konkretne vrednosti za a i b. -> ne vazi (x #xc0) (y #x40)

(set-logic QF_BV)

(declare-const x (_ BitVec 8))
(declare-const y (_ BitVec 8))

(define-fun min60 () (_ BitVec 8) 
    (bvneg (_ bv60 8))
)

(define-fun max60 () (_ BitVec 8)
    (_ bv60 8)
)

; (assert
;     (and
;         (bvsle min60 x)
;         (bvsle x max60)
;     )
; )
; 
; (assert
;     (and
;         (bvsle min60 y)
;         (bvsle y max60)
;     )
; )

(define-fun clamp_abs_diff ((a (_ BitVec 8)) (b (_ BitVec 8))) (_ BitVec 8)
    (let ((diff (bvsub a b)))
        (let ((abs_d (ite (bvslt diff (_ bv0 8)) (bvneg diff) diff))) 
            (ite (bvsgt abs_d (_ bv10 8)) (_ bv10 8) abs_d)
        )
    )
)

; (assert 
;     (or
;         (bvslt (clamp_abs_diff x y) (_ bv0 8))
;         (bvsgt (clamp_abs_diff x y) (_ bv10 8))
;     )
; )

; (assert
;     (and
;         (= (clamp_abs_diff x y) (_ bv0 8))
;         (distinct x y)
;     )
; )

(assert 
    (or
        (bvslt (clamp_abs_diff x y) (_ bv0 8))
        (bvsgt (clamp_abs_diff x y) (_ bv10 8))
    )
)

(check-sat)
(get-value (x y))
(exit)