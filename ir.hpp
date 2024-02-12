#pragma once
#include <iostream>
#include <string>
#include <vector>

enum SimIROp {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_ASSIGN,
  OP_MUL_ASSIGN,
  OP_DIV_ASSIGN,
  OP_MOD_ASSIGN,
  OP_ADD_ASSIGN,
  OP_SUB_ASSIGN,
  OP_LEFT_ASSIGN,
  OP_RIGHT_ASSIGN,
  OP_AND_ASSIGN,
  OP_XOR_ASSIGN,
  OP_OR_ASSIGN,
  OP_IF,
  OP_WHILE,
  OP_GOTO,
  OP_LABEL,
  OP_LT,
  OP_GT,
  OP_LE,
  OP_GE,
  OP_EQ,
  OP_NE,
  OP_AND,
  OP_OR,
  OP_NOT,
  OP_NEG,
  OP_RETURN,
  OP_CALL,
  OP_PARAM,
  OP_ARG,
  OP_FUNCTION,
  OP_END_FUNCTION,
  OP_ARRAY,
  OP_INDEX,
  OP_DEREF,
  OP_ADDR,
  OP_CAST,
  OP_SIZEOF,
  OP_EXIT,
  OP_START,
  OP_END,
  OP_NOP,
  OP_HALT,
  OP_ERROR,
  OP_LAST
};

class SimIRInst {
public:
  enum SimIROp op;
  std::string arg1;
  std::string arg2;
  std::string result;

  void emit(std::ostream &os) const {
    os << op << " " << arg1 << " " << arg2 << " " << result << std::endl;
  }

  static SimIRInst add(const std::string &arg1, const std::string &arg2,
                       const std::string &result) {
    return {OP_ADD, arg1, arg2, result};
  }

  static SimIRInst sub(const std::string &arg1, const std::string &arg2,
                       const std::string &result) {
    return {OP_SUB, arg1, arg2, result};
  }

  static SimIRInst mul(const std::string &arg1, const std::string &arg2,
                       const std::string &result) {
    return {OP_MUL, arg1, arg2, result};
  }

  static SimIRInst div(const std::string &arg1, const std::string &arg2,
                       const std::string &result) {
    return {OP_DIV, arg1, arg2, result};
  }

  static SimIRInst assign(const std::string &arg1, const std::string &result) {
    return {OP_ASSIGN, arg1, "", result};
  }

  static SimIRInst mul_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_MUL_ASSIGN, arg1, result};
  }

  static SimIRInst div_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_DIV_ASSIGN, arg1, result};
  }

  static SimIRInst mod_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_MOD_ASSIGN, arg1, result};
  }

  static SimIRInst add_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_ADD_ASSIGN, arg1, result};
  }

  static SimIRInst sub_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_SUB_ASSIGN, arg1, result};
  }

  static SimIRInst left_assign(const std::string &arg1,
                               const std::string &result) {
    return {OP_LEFT_ASSIGN, arg1, result};
  }

  static SimIRInst right_assign(const std::string &arg1,
                                const std::string &result) {
    return {OP_RIGHT_ASSIGN, arg1, result};
  }

  static SimIRInst and_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_AND_ASSIGN, arg1, result};
  }

  static SimIRInst xor_assign(const std::string &arg1,
                              const std::string &result) {
    return {OP_XOR_ASSIGN, arg1, result};
  }

  static SimIRInst or_assign(const std::string &arg1,
                             const std::string &result) {
    return {OP_OR_ASSIGN, arg1, result};
  }

  static SimIRInst lt(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_LT, arg1, arg2, result};
  }

  static SimIRInst gt(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_GT, arg1, arg2, result};
  }

  static SimIRInst le(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_LE, arg1, arg2, result};
  }

  static SimIRInst ge(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_GE, arg1, arg2, result};
  }

  static SimIRInst eq(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_EQ, arg1, arg2, result};
  }

  static SimIRInst ne(const std::string &arg1, const std::string &arg2,
                      const std::string &result) {
    return {OP_NE, arg1, arg2, result};
  }

  static SimIRInst and_(const std::string &arg1, const std::string &arg2,
                        const std::string &result) {
    return {OP_AND, arg1, arg2, result};
  }

  static SimIRInst or_(const std::string &arg1, const std::string &arg2,
                       const std::string &result) {
    return {OP_OR, arg1, arg2, result};
  }

  static SimIRInst not_(const std::string &arg1, const std::string &result) {
    return {OP_NOT, arg1, "", result};
  }

  static SimIRInst neg(const std::string &arg1, const std::string &result) {
    return {OP_NEG, arg1, "", result};
  }

  static SimIRInst return_(const std::string &arg1) {
    return {OP_RETURN, arg1, "", ""};
  }

  static SimIRInst call(const std::string &arg1, const std::string &result) {
    return {OP_CALL, arg1, "", result};
  }

  static SimIRInst param(const std::string &arg1) {
    return {OP_PARAM, arg1, "", ""};
  }

  static SimIRInst arg(const std::string &arg1) {
    return {OP_ARG, arg1, "", ""};
  }

  static SimIRInst function(const std::string &arg1) {
    return {OP_FUNCTION, arg1, "", ""};
  }

  static SimIRInst end_function() { return {OP_END_FUNCTION, "", "", ""}; }

  static SimIRInst array(const std::string &arg1, const std::string &arg2,
                         const std::string &result) {
    return {OP_ARRAY, arg1, arg2, result};
  }

  static SimIRInst index(const std::string &arg1, const std::string &arg2,
                         const std::string &result) {
    return {OP_INDEX, arg1, arg2, result};
  }

  //   OP_ARRAY,
  //   OP_INDEX,
  //   OP_DEREF,
  //   OP_ADDR,
  //   OP_CAST,
  //   OP_SIZEOF,
  //   OP_EXIT,
  //   OP_START,
  //   OP_END,
  //   OP_NOP,
  //   OP_HALT,
  //   OP_ERROR,
  //   OP_LAST
};

class SimIR {
public:
  void push_back(const SimIRInst &ir) { irs.push_back(ir); }

  void emit(std::ostream &os) const {
    for (auto &&ir : irs) {
      ir.emit(os);
    }
  }

private:
  std::vector<SimIRInst> irs;
};
