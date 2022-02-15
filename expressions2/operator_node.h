#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include <glog/logging.h>

#include <cmath>

#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right);
  explicit inline OperatorNode(std::istream &in) : operator_('?') {
    in >> operator_;
    left_ = std::move(Expression<T>(in));
    right_ = std::move(Expression<T>(in));
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(const OperatorNode<T> &from);
  OperatorNode(OperatorNode<T> &&from) noexcept = delete;
  ~OperatorNode() = default;
  OperatorNode<T> &operator=(OperatorNode<T> &from) = delete;
  OperatorNode<T> &operator=(OperatorNode<T> &&from) noexcept = delete;

  [[nodiscard]] T Eval() {
    T left = left_.Eval();
    T right = right_.Eval();
    switch (operator_) {
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
        LOG_ASSERT(false) << "Unexpected operator: " << operator_;
    }
  }

  void PrintAsTree(int indent);
  [[nodiscard]] std::string ToStringWithParen();
  void ToStream(std::ostream &out);

private:
  Expression<T> left_;
  char operator_;
  Expression<T> right_;
};

}  // namespace code_experiments

#endif