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
  explicit Expression(const std::string& expr);
  explicit inline Expression(std::istream& in)
      : simplified_value_(-1),
        operator_node_cache_(nullptr) {
    std::string specifier;
    in >> specifier;
    if (specifier == kBOperatorStr || specifier == kTOperatorStr) {
      operator_node_cache_ = std::make_unique<OperatorNode<T>>(in, specifier);
    } else {
      CHECK(specifier == kConstantStr) << "Unexpected: " << specifier;
      std::string expr;
      in >> expr;
      simplified_value_ = GetSimplifiedVal(expr);
    }
  }

  Expression<T>& operator=(Expression<T>&& from) noexcept;
  Expression(Expression<T>&& from) noexcept;
  Expression();
  Expression(Expression<T>&& left, char op, Expression<T>&& right);
  Expression(
      char op,
      Expression<T>&& ternary,
      Expression<T>&& left,
      Expression<T>&& right);
  Expression(T constant);
  Expression(const Expression<T>& from);

  ~Expression() = default;
  Expression<T>& operator=(Expression<T>& from) = delete;

  [[nodiscard]] inline T Eval() {
    return (operator_node_cache_ != nullptr) ? operator_node_cache_->Eval()
                                             : simplified_value_;
  }

  void PrintAsTree(int indent);
  std::string ToStringWithParen();
  void ToStream(std::ostream& out);

private:
  [[nodiscard]] int GetOperatorIndex(const std::string& expr) const;
  void Parse(const std::string& expr);
  [[nodiscard]] T GetSimplifiedVal(const std::string& expr) const;

  T simplified_value_;
  std::unique_ptr<OperatorNode<T>> operator_node_cache_;
};

}  // namespace code_experiments

#endif