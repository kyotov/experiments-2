#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include <glog/logging.h>

#include <algorithm>
#include <cmath>

#include "consts.h"
#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right);
  inline OperatorNode(std::istream &in, const std::string &specifier) {
    int num_operands = 2;
    if (specifier == kTOperatorStr) {
      op_ = "?";
      num_operands = 3;
    } else if (specifier == kFxOperatorStr) {
      in >> op_;
      in >> num_operands;
    } else {
      in >> op_;
    }
    for (int i = 0; i < num_operands; i++) {
      operands_.emplace_back(in);
    }
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(
      char op,
      Expression<T> &&ternary,
      Expression<T> &&left,
      Expression<T> &&right);

  [[nodiscard]] inline T Eval() const {
    T left = operands_[0].Eval();
    T right = operands_[1].Eval();
    switch (op_[0]) {
      case '+':
        return left + right;
      case '-':
        return left - right;
      case '*':
        return left * right;
      case '/':
        return left / right;
      case '^':
        return std::pow(left, right);
      default:
        return EvalFunc();
    }
  }

  void PrintAsTree(int indent) const;

  [[nodiscard]] std::string ToStringWithParen() const;
  void ToStream(std::ostream &out) const;

private:
  [[nodiscard]] T EvalFunc() const;
  [[nodiscard]] std::string FuncToStringWithParen() const;

  std::string op_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif