#pragma once

#include <iostream>

#include "axioms.h"
#include "tcc.h"
#include "bv2int.h"
#include "preprocessor.h"
#include "postprocessor.h"
#include "smt-switch/result.h"
#include "smt-switch/smt.h"

namespace lbv2i {

class LBV2ISolver : public smt::AbsSmtSolver
{
 public:
  LBV2ISolver(smt::SmtSolver & solver, bool lazy = false);
  ~LBV2ISolver();

  smt::Result solve();

  void push(uint64_t num = 1) override;
  void pop(uint64_t num = 1) override;
  void reset() override;
  void reset_assertions() override;
  void set_logic(const std::string logic_name) override;
  void set_opt(std::string op, std::string value) override;
  void assert_formula(const smt::Term & f) override;
  void do_assert_formula();
  void dump_smt2();
  smt::Term get_value(const smt::Term & t) const override;
  smt::UnorderedTermMap get_array_values(
      const smt::Term & arr, smt::Term & out_const_base) const override;
  smt::Result check_sat() override;
  smt::Result check_sat_assuming(const smt::TermVec & assumptions) override;

  void get_unsat_assumptions(smt::UnorderedTermSet & core) override;

  smt::Term substitute(
      const smt::Term term,
      const smt::UnorderedTermMap & substitution_map) const override;

  smt::Sort make_sort(const smt::SortKind sk) const override;
  smt::Sort make_sort(const std::string name, uint64_t arity) const override;
  smt::Sort make_sort(const smt::SortKind sk, uint64_t size) const override;
  smt::Sort make_sort(const smt::SortKind sk,
                      const smt::Sort & sort1) const override;
  smt::Sort make_sort(const smt::SortKind sk,
                      const smt::Sort & sort1,
                      const smt::Sort & sort2) const override;
  smt::Sort make_sort(const smt::SortKind sk,
                      const smt::Sort & sort1,
                      const smt::Sort & sort2,
                      const smt::Sort & sort3) const override;
  smt::Sort make_sort(const smt::SortKind sk,
                      const smt::SortVec & sorts) const override;
  smt::Sort make_sort(const smt::Sort & sort_con,
                      const smt::SortVec & sorts) const override;

  smt::Term make_symbol(const std::string name,
                        const smt::Sort & sort) override;
  smt::Term make_param(const std::string name, const smt::Sort & sort) override;
  smt::Term make_term(const smt::Op op,
                      const smt::TermVec & terms) const override;
  smt::Term make_term(const smt::Op op, const smt::Term & t) const override;
  smt::Term make_term(const smt::Op op,
                      const smt::Term & t0,
                      const smt::Term & t1) const override;
  smt::Term make_term(const smt::Op op,
                      const smt::Term & t0,
                      const smt::Term & t1,
                      const smt::Term & t2) const override;
  smt::Term make_term(const std::string val,
                      const smt::Sort & sort,
                      uint64_t base = 10) const override;

  smt::Term make_term(bool b) const override;
  smt::Term make_term(int64_t i, const smt::Sort & sort) const override;
  smt::Term make_term(const smt::Term & val,
                      const smt::Sort & sort) const override;

  smt::Sort make_sort(const smt::DatatypeDecl & d) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  smt::DatatypeDecl make_datatype_decl(const std::string & s) override
  {
    throw NotImplementedException("Not Implemented");
  };
  smt::DatatypeConstructorDecl make_datatype_constructor_decl(
      const std::string s) override
  {
    throw NotImplementedException("Not Implemented");
  };
  void add_constructor(smt::DatatypeDecl & dt,
                       const smt::DatatypeConstructorDecl & con) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  void add_selector(smt::DatatypeConstructorDecl & dt,
                    const std::string & name,
                    const smt::Sort & s) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  void add_selector_self(smt::DatatypeConstructorDecl & dt,
                         const std::string & name) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  smt::Term get_constructor(const smt::Sort & s,
                            std::string name) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  smt::Term get_tester(const smt::Sort & s, std::string name) const override
  {
    throw NotImplementedException("Not Implemented");
  };
  smt::Term get_selector(const smt::Sort & s,
                         std::string con,
                         std::string name) const override
  {
    throw NotImplementedException("Not Implemented");
  };

  void run(std::string filename);
  void run_on_stdin();

 private:
  bool refine(smt::TermVec & outlemmas);
  bool refine_bvand(const smt::TermVec & fterms, smt::TermVec & outlemmas);

  void refine_final_shift(const smt::TermVec & fterms,
                          smt::TermVec & outlemmas);

  bool refine_bvlshift(const smt::TermVec & fterms, smt::TermVec & outlemmas);
  bool refine_bvrshift(const smt::TermVec & fterms, smt::TermVec & outlemmas);
  bool refine_final_bw(smt::Op op, const smt::TermVec & fterms,
                       smt::TermVec & outlemmas);

  bool try_sat_check(smt::TermVec &outlemmas);

  // print result and values based on options
  void print_result(smt::Result res) const;

  // BV2Int Translator
  BV2Int * bv2int_;

  // Preprocessor that will eliminate some bv operators. Note: keep in mind
  // while writing the preprocessor that we want to use it also in the
  // incremental setting (push/pop)
  Preprocessor * prepro_;
  

  // Postprocessor that will eliminate some integer operators. Note: keep in mind
  // while writing the postprocessor that we want to use it also in the
  // incremental setting (push/pop)
  Postprocessor * postpro_;

  // axioms for refinement
  Axioms axioms_;

  // smt-switch solver
  smt::SmtSolver & solver_;
  smt::SmtSolver s_checker_;
  // term transfer to s_checker
  smt::TermTranslator tr_s_checker_;
  // base assumption for s_checker
  smt::Term s_checker_base_assump_;

  // assertion stack
  smt::TermVec orig_assertions_;
  smt::TermVec assertions_;
  smt::TermVec extra_assertions_;
  size_t last_asserted_size_;

  // stack 
  typedef std::tuple<size_t, size_t, size_t> stack_entry_t;
  std::vector<stack_entry_t> stack_;

  bool lazy_;
};

}  // namespace lbv2i
