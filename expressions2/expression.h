#ifndef EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H
#define EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H

#include <iostream>
#include <memory>
#include <string>

#include "bin_operator_node.h"
#include "operator_node.h"

namespace code_experiments {

template <typename T>
class OperatorNode;

template <typename T>
class BinOperatorNode;

template <typename T>
class Expression {
public:
  explicit Expression(const std::string& expr);
  explicit Expression(std::istream& in)
      : simplified_value_(-1),
        bin_operator_node_cache_(nullptr),
        operator_node_cache_(nullptr) {
    std::string specifier;
    in >> specifier;
    if (specifier == kConstantStr) {
      std::string expr;
      in >> expr;
      simplified_value_ = GetSimplifiedVal(expr);
    } else if (specifier == kBOperatorStr) {
      bin_operator_node_cache_ = std::make_unique<BinOperatorNode<T>>(in);
    } else {
      operator_node_cache_ = std::make_unique<OperatorNode<T>>(in, specifier);
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
  Expression(const Expression<T>& from) = delete;
  ~Expression() = default;
  Expression<T>& operator=(Expression<T>& from) = delete;

  [[nodiscard]] T Eval() const {
    return (bin_operator_node_cache_ != nullptr)
               ? bin_operator_node_cache_->Eval()
               : ((operator_node_cache_ == nullptr)
                      ? simplified_value_
                      : operator_node_cache_->Eval());
  }

  void PrintAsTree(int indent) const;

  std::string ToStringWithParen() const {
    return (bin_operator_node_cache_ != nullptr)
               ? bin_operator_node_cache_->ToStringWithParen()
               : ((operator_node_cache_ != nullptr)
                      ? operator_node_cache_->ToStringWithParen()
                      : "(" + std::to_string(simplified_value_) + ")");
  }

  void ToStream(std::ostream& out) const;

private:
  [[nodiscard]] int GetOperatorIndex(const std::string& expr) const;
  void Parse(const std::string& expr);
  [[nodiscard]] T GetSimplifiedVal(const std::string& expr) const;

  T simplified_value_;
  std::unique_ptr<OperatorNode<T>> operator_node_cache_;
  std::unique_ptr<BinOperatorNode<T>> bin_operator_node_cache_;
};

}  // namespace code_experiments

#endif