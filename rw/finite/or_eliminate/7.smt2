
(set-logic QF_BV)
(declare-fun x () (_ BitVec 7))
(declare-fun y () (_ BitVec 7))
(define-fun left () (_ BitVec 7) (bvor x y))
(define-fun right () (_ BitVec 7) (bvsub (bvadd x y) (bvand x y)))
(assert (distinct left right))
(check-sat)
