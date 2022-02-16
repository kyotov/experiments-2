#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include <glog/logging.h>

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
  inline OperatorNode(std::istream &in, const std::string &specifier)
      : operator_('?') {
    if (specifier == kTOperatorStr) {
      operands_.emplace_back((in));
    } else {
      in >> operator_;
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
  OperatorNode(const OperatorNode<T> &from);
  OperatorNode(OperatorNode<T> &&from) noexcept = delete;
  ~OperatorNode() = default;
  OperatorNode<T> &operator=(OperatorNode<T> &from) = delete;
  OperatorNode<T> &operator=(OperatorNode<T> &&from) noexcept = delete;

  [[nodiscard]] inline T Eval() {
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
      case '?':
        return operands_[0].Eval() ? left : right;
      default:
        LOG_ASSERT(false) << "Unexpected operator: " << operator_;
    }
  }
  void PrintAsTree(int indent);

  [[nodiscard]] inline std::string ToStringWithParen() {
    if (operator_ == '?') {
      return "(" + operands_[0].ToStringWithParen() + "?" +
             left_.ToStringWithParen() + ":" + right_.ToStringWithParen() + ")";
    } else {
      return "(" + left_.ToStringWithParen() + operator_ +
             right_.ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream &out);

private:
  Expression<T> left_;
  char operator_;
  Expression<T> right_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif