
(set-logic QF_BV)
(declare-fun x () (_ BitVec 37))
(declare-fun y () (_ BitVec 37))
(define-fun mins () (_ BitVec 37) (_ bv68719476736 37))
(define-fun negative ((a (_ BitVec 37))) Bool (bvuge a (_ bv68719476736 37)))
(define-fun abs ((a (_ BitVec 37))) (_ BitVec 37) (ite (negative a) (bvneg a) a))
(define-fun u ((a (_ BitVec 37)) (b (_ BitVec 37))) (_ BitVec 37) (bvurem (abs a) (abs b)))
(define-fun neg_u ((a (_ BitVec 37)) (b (_ BitVec 37))) (_ BitVec 37) (bvneg (u a b)))
(define-fun cond_0 () Bool (= (u x y) (_ bv0 37)))
(define-fun cond_1 () Bool (and (not (negative x)) (not (negative y))))
(define-fun cond_2 () Bool (and (negative x) (not (negative y))))
(define-fun cond_3 () Bool (and (not (negative x)) (negative y)))
(define-fun left () (_ BitVec 37) (bvsmod x y))
(define-fun right () (_ BitVec 37) (ite cond_0 (u x y) (ite cond_1 (u x y) (ite cond_2 (bvadd (neg_u x y) y) (ite cond_3 (bvadd (u x y) y) (neg_u x y))))))
(assert (distinct left right))
(check-sat)
