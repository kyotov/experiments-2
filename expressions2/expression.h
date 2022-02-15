#ifndef EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H
#define EXPERIMENTS_EXPRESSIONS2_EXPRESSION_H

#include <iostream>
#include <memory>
#include <string>

#include "consts.h"
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
    if (specifier == kBOperatorStr) {
      operator_node_cache_ = std::make_unique<OperatorNode<T>>(in);
    } else {
      std::string expr;
      in >> expr;
      simplified_value_ = GetSimplifiedVal(expr);
    }
  }

  Expression<T>& operator=(Expression<T>&& from) noexcept;
  Expression(Expression<T>&& from) noexcept;
  Expression();
  Expression(Expression<T>&& left, char op, Expression<T>&& right);
  Expression(T constant);
  Expression(const Expression<T>& from);

  ~Expression() = default;
  Expression<T>& operator=(Expression<T>& from) = delete;

  [[nodiscard]] T Eval() {
    return (operator_node_cache_ != nullptr) ? operator_node_cache_->Eval()
                                             : simplified_value_;
  }

  void PrintAsTree(int indent);
  std::string ToStringWithParen();
  void ToStream(std::ostream& out);

private:
  [[nodiscard]] int GetOperatorIndex(const std::string& expr) const;
  void Parse(const std::string& expr);
  [[nodiscard]] inline T GetSimplifiedVal(const std::string& expr) const {
    return expr.empty() ? 0 : std::stoi(expr);
  }

  T simplified_value_;
  std::unique_ptr<OperatorNode<T>> operator_node_cache_;
};

}  // namespace code_experiments

#endif