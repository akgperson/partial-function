
(set-logic QF_BV)
(declare-fun x () (_ BitVec 2))
(declare-fun y () (_ BitVec 2))
(define-fun left () (_ BitVec 2) (bvor x y))
(define-fun right () (_ BitVec 2) (bvsub (bvadd x y) (bvand x y)))
(assert (distinct left right))
(check-sat)
