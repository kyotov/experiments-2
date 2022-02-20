#ifndef EXPERIMENTS_EXPRESSIONS2_BIN_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_BIN_OPERATOR_NODE_H

#include <glog/logging.h>

#include <algorithm>
#include <cmath>

#include "consts.h"
#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class BinOperatorNode {
public:
  BinOperatorNode(const std::string &left, char op, const std::string &right);
  BinOperatorNode(std::istream &in) {
    in >> op_;
    left_ = std::move(Expression<T>(in));
    right_ = std::move(Expression<T>(in));
  }

  BinOperatorNode(Expression<T> &&left, char op, Expression<T> &&right);

  [[nodiscard]] T Eval() const {
    T left = left_.Eval();
    T right = right_.Eval();
    switch (op_) {
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
        CHECK(false) << "Invalid binary operator " << op_;
    }
  }

  void PrintAsTree(int indent) const;

  [[nodiscard]] std::string ToStringWithParen() const;
  void ToStream(std::ostream &out) const;

private:
  char op_;
  Expression<T> left_;
  Expression<T> right_;
};

}  // namespace code_experiments

#endif