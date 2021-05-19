#include "smt-switch/identity_walker.h"
#include "smt-switch/tree_walker.h"

class TCCGenerator : smt::TreeWalker
{
  public:
    TCCGenerator(smt::SmtSolver &solver, bool b); //b option:
    ~TCCGenerator();
//    smt::TreeWalkerStepResult visit_term(smt::Term &formula, smt::Term &term, std::vector<int> &path) override; //check Term and Term what input and what output? math to function overriding? New ones?
    smt::Term generate_tcc(smt::Term &t); //in parenthesis? what feed to my function?
    smt::Term convert(smt::Term &t);
    int counter=0;

    int b_iter{};
    // list of all fresh variables to and at top level of tcc
    smt::Term fresh_bns;
    int n=0;

    std::vector<smt::Term> cached_children;
    std::vector<smt::Term> cached_tcc;

    using TermVec = std::vector<smt::Term>;
    smt::Term int_zero_;
    smt::Term bool_true_;

    using UnorderedTermMap = std::unordered_map<smt::Term, smt::Term>;
    smt::UnorderedTermMap tcc_cache_;
};

