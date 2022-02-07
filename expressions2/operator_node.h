#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right)
      : left_(left),
        operator_(op),
        right_(right) {}
  [[nodiscard]] T Eval();
  void PrintAsTree(int indent);
  [[nodiscard]] std::string ToStringWithParen();
  void ToStream(std::ostream &out);

private:
  static const char kSeparator = ' ';

  Expression<T> left_;
  char operator_;
  Expression<T> right_;
};

}  // namespace code_experiments

#endif