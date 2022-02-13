#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right);
  OperatorNode(std::istream &in, const std::string &specifier);
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

  [[nodiscard]] T Eval();
  void PrintAsTree(int indent);
  [[nodiscard]] std::string ToStringWithParen();
  void ToStream(std::ostream &out);

private:
  Expression<T> left_;
  char operator_;
  Expression<T> right_;
  // NOTE: Further extension will likely be through n-ary form.
  Expression<T> ternary_;
};

}  // namespace code_experiments

#endif