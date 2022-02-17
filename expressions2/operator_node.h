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
    if (specifier == kTOperatorStr) {
      operands_.emplace_back((in));
      op_ = "?";
    } else if (specifier == kFxOperatorStr) {
      in >> op_;
      int size;
      in >> size;
      for (int i = 0; i < size; i++) {
        operands_.emplace_back(in);
      }
      return;
    } else {
      in >> op_;
    }
    operands_.emplace_back(in);
    operands_.emplace_back(in);
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(
      char op,
      Expression<T> &&ternary,
      Expression<T> &&left,
      Expression<T> &&right);

  [[nodiscard]] T EvalFunc() const;

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

  void PrintAsTree(int indent);

  [[nodiscard]] std::string FuncToStringWithParen();

  [[nodiscard]] inline std::string ToStringWithParen() {
    if (op_.size() > 1) {
      return FuncToStringWithParen();
    } else if (op_[0] == '?') {
      return "(" + operands_[0].ToStringWithParen() + "?" +
             operands_[1].ToStringWithParen() + ":" +
             operands_[2].ToStringWithParen() + ")";
    } else {
      return "(" + operands_[0].ToStringWithParen() + op_[0] +
             operands_[1].ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream &out);

private:
  std::string op_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif