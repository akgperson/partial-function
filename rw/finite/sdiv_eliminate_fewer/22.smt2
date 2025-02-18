
(set-logic QF_BV)
(declare-fun x () (_ BitVec 22))
(declare-fun y () (_ BitVec 22))
(define-fun mins () (_ BitVec 22) (_ bv2097152 22))

(define-fun negative ((a (_ BitVec 22))) Bool (bvuge a (_ bv2097152 22)))
(define-fun abs ((a (_ BitVec 22))) (_ BitVec 22) (ite (negative a) (bvneg a) a))
(define-fun abs_div ((a (_ BitVec 22)) (b (_ BitVec 22))) (_ BitVec 22) (bvudiv (abs a) (abs b)))
(define-fun left () (_ BitVec 22) (bvsdiv x y))
(define-fun right () (_ BitVec 22) (ite (xor (negative x) (negative y)) (bvneg (abs_div x y)) (abs_div x y)))
(assert (distinct left right))
(check-sat)
