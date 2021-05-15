#include "tcc.h"
using namespace smt;
using namespace std;

TCCGenerator::TCCGenerator(SmtSolver &solver, bool b) : TreeWalker(solver, b)
{
  counter = 0;
  n = 0;
  Sort int_sort = solver_->make_sort(INT);
  int_zero_ = solver_->make_term(0, int_sort);
};

TCCGenerator::~TCCGenerator() { };

smt::TreeWalkerStepResult TCCGenerator::visit_term(smt::Term &formula, smt::Term &t, std::vector<int> &path){
  Sort boolsort = solver_->make_sort(BOOL); //TODO honestly uncertain abt this line

  Op op = t->get_op();

  if (!op.is_null()){

    TermVec cached_children;
    TermVec cached_tcc;

    for (auto c : t){
      cached_children.push_back(c);
      cached_tcc.push_back(tcc_cache_.at(c));
    }

    if (op.prim_op == Or) {
      Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
      tcc_cache_[t] = condition;
    }

    else if (op.prim_op == And) {
      Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
      tcc_cache_[t] = condition;
    }
    else if (op.prim_op == Ite) {
      Term condition = solver_->make_term(And, cached_tcc[0], solver_->make_term(Ite, cached_children[0], cached_tcc[1], cached_tcc[2]));
      tcc_cache_[t] = condition;
    }
    else if (op.prim_op == Implies) {
      Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
      tcc_cache_[t] = condition;
    }
    else if (op.prim_op == Not) {
      Term condition = cached_tcc[0];
      tcc_cache_[t] = condition;
    }
//    else if (op.prim_op == Iff) {
  //    Term subcondition1 = solver_->make_term(And, cached_children[0], cached_children[1]);
    //  Term subcondition2 = solver_->make_term(And, solver_->make_term(Not, cached_children[0]), solver_->make_term(Not, cached_children[1]));
      //Term subcondition3 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
      //Term subcondition4 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
      //Term condition = solver_->make_term(Or, solver_->make_term(And, subcondition1, subcondition3), solver_->make_term(And, subcondition4, subcondition2), solver_->make_term(And, subcondition3, subcondition4));
      //tcc_cache_[t] = condition;
    //}

    //TODO add quantifiers

    else if (op.prim_op == Mod || op.prim_op == Mod || op.prim_op == Mod) {
      string var_name = string("b") + to_string(b_iter);
      Term b = solver_->make_symbol(var_name, boolsort);
      b_iter++;

      pair<Term, vector<int>> occ;
      occ.first = formula;
      occ.second = path;

      save_in_cache(b, occ);

      fresh_bns = solver_->make_term(And, fresh_bns, solver_->make_term(Equal, b, bool_true_));

      Term fresh_condition = solver_->make_term(Equal, b, solver_->make_term(Distinct, cached_children[1], int_zero_));
      Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], fresh_condition);
      tcc_cache_[t] = condition;
    }

    else { //op w/o undefined behavior
      Term condition;
      if (cached_tcc.size() == 1) {
        condition = cached_tcc[0];
      }
      else {
        condition = solver_->make_term(And, cached_tcc);
      }
      tcc_cache_[t] = condition;
    }
  }
  else { //at leaf
      tcc_cache_[t] = solver_->make_term(true); //change
  }

  // add fresh bns to topmost node
  // TODO could this have problems... will a sub node ever be the same as the top node... no, right?
  if (formula == t){ //TODO this equality all right?
    Term old_topmost = tcc_cache_[t];
    tcc_cache_[t] = solver_->make_term(And, old_topmost, fresh_bns);
  }
  return TreeWalker_Continue;
}

//smt::TreeWalkerStepResult TCCGenerator::visit_term(smt::Term &formula, smt::Term &t, std::vector<int> &path)
//{
  //if (!preorder_) { //if current term not being visited for first time (on wind out) [Note: preorder_ true when current term is being visited for the first time]
    //Op op = t->get_op();
/*
    if (!op.is_null()) {

      TermVec cached_children;
      TermVec cached_tcc;
      //array  fresh_pns;

      for (auto c : t) {
        cached_children.push_back(c);
        cached_tcc.push_back(cache_.at(c));
      }

      if (op.prim_op == Or) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == And) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Ite) {
        Term condition = solver_->make_term(And, cached_tcc[0], solver_->make_term(Ite, cached_children[0], cached_tcc[1], cached_tcc[2]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Implies) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Not) {
        Term condition = cached_tcc[0];
        cache_[t] = condition;
      }
      else if (op.prim_op == Iff) {
        Term subcondition1 = solver_->make_term(And, cached_children[0], cached_children[1]);
        Term subcondition2 = solver_->make_term(And, solver_->make_term(Not, cached_children[0]), solver_->make_term(Not, cached_children[1]));
        Term subcondition3 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        Term subcondition4 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        Term condition = solver_->make_term(Or, solver_->make_term(And, subcondition1, subcondition3), solver_->make_term(And, subcondition4, subcondition2), solver_->make_term(And, subcondition3, subcondition4));
        cache_[t] = condition;
      }

      else if (op.prim_op == Mod) { //could merge w Div
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition;
      }
      else if (op.prim_op == Div) {
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition; //change
      }
      else if (op.prim_op == IntDiv) { //could merge w Div
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition; //change
      }

      //Quantifiers
//      else if (op.prim_op == Exists) {
        // Exists x T <-> (Exists x (T ^ TCC(T)) \/ (Forall x TCC(T))
//        Term condition = solver_->make_term(Or, solver_->make_term(Exists, cached_children[0], solver_->make_term(And, cached_children[1], cached_tcc[1]), solver_->make_term(Forall, cached_children[0], cached_tcc[1])));
//        cache_[t] = condition;
//      }
//      else if (op.prim_op == Forall) {
        // (forall (x) term)
        // forall (x) T <=> ~(Exists (x) ~T)
        // forall x T <-> (Exists x (~T ^ TCC(T))) \/ (Forall x TCC(T))
//        Term condition = solver_->make_term(Or, solver_->make_term(Exists, cached_children[0], solver_->make_term(And, solver_->make_term(Not, cached_children[1]), cached_tcc[1])), solver_->make_term(Forall, cached_children[0], cached_tcc[1]));
//        cache_[t] = condition;
//      }

      else { //op w/o undefined behavior
        Term condition;
        if (cached_tcc.size() == 1) {
          condition = cached_tcc[0];
        }
        else {
          condition = solver_->make_term(And, cached_tcc);
        }
//        will need loop for non binary ops?
        cache_[t] = condition;
      }
    }
    else { //at leaf
        cache_[t] = solver_->make_term(true); //change
    }
  }
  return Walker_Continue;
}
*/
smt::Term TCCGenerator::convert(smt::Term &t)
{
  visit(t);
  return t;
//  Term res = cache_.at(t);
//  cout << "res = " << res << endl;
//  return res;
}

smt::Term TCCGenerator::generate_tcc(smt::Term &t)
{
  //return new_tcc;
  //TCC return true iff formula undefined; look into recursive def and way to build of TCC to understand in general
  cout << t << endl;
  return t;
}
