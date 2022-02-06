#ifndef EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H
#define EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H

#include <iostream>
#include <memory>
#include <string>

#include "operator_node.h"

namespace code_experiments {

template <typename T>
class OperatorNode;

template <typename T>
class Expression {
public:
  explicit Expression(const std::string& expr) : operator_node_cache_(nullptr) {
    result_ = ParseAndEvaluate(expr);
  }
  [[nodiscard]] T Eval() { return result_; }
  void PrintAsTree(int indent);
  std::string ToStringWithParen();

private:
  [[nodiscard]] int GetOperatorIndex(const std::string& expr) const;
  [[nodiscard]] T ParseAndEvaluate(const std::string& expr);

  T result_;
  std::unique_ptr<OperatorNode<T>> operator_node_cache_;
};

}  // namespace code_experiments

#endif