
(set-logic QF_BV)
(declare-fun x () (_ BitVec 10))
(declare-fun y () (_ BitVec 10))
(define-fun left () (_ BitVec 10) (bvxor x y))
(define-fun right () (_ BitVec 10) (bvsub (bvor x y) (bvand x y)))
(assert (distinct left right))
(check-sat)
