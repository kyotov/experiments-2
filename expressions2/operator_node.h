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
    left_ = std::move(Expression<T>(in));
    right_ = std::move(Expression<T>(in));
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(
      char op,
      Expression<T> &&ternary,
      Expression<T> &&left,
      Expression<T> &&right);

  [[nodiscard]] T EvalFunc() const;

  [[nodiscard]] inline T Eval() const {
    T left = left_.Eval();
    T right = right_.Eval();
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
      case '?':
        return operands_[0].Eval() ? left : right;
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
             left_.ToStringWithParen() + ":" + right_.ToStringWithParen() + ")";
    } else {
      return "(" + left_.ToStringWithParen() + op_[0] +
             right_.ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream &out);

private:
  Expression<T> left_;
  Expression<T> right_;
  std::string op_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif