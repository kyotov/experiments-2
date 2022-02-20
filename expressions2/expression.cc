#include "expression.h"

#include <glog/logging.h>

#include "../cast_utils/cast_utils.h"
#include "consts.h"

namespace code_experiments {

template <typename T>
Expression<T>::Expression(const std::string& expr)
    : simplified_value_(-1),
      bin_operator_node_cache_(nullptr),
      operator_node_cache_(nullptr) {
  Parse(expr);
}

template <typename T>
Expression<T>& Expression<T>::operator=(Expression<T>&& from) noexcept {
  simplified_value_ = from.simplified_value_;
  operator_node_cache_ = std::move(from.operator_node_cache_);
  bin_operator_node_cache_ = std::move(from.bin_operator_node_cache_);
  return *this;
}

template <typename T>
Expression<T>::Expression(Expression<T>&& from) noexcept
    : simplified_value_(from.simplified_value_),
      bin_operator_node_cache_(std::move(from.bin_operator_node_cache_)),
      operator_node_cache_(std::move(from.operator_node_cache_)) {}

template <typename T>
Expression<T>::Expression(T constant)
    : simplified_value_(constant),
      bin_operator_node_cache_(nullptr),
      operator_node_cache_(nullptr) {}

template <typename T>
Expression<T>::Expression(Expression<T>&& left, char op, Expression<T>&& right)
    : simplified_value_(-1),
      bin_operator_node_cache_(std::make_unique<BinOperatorNode<T>>(
          std::move(left),
          op,
          std::move(right))),
      operator_node_cache_(nullptr) {}

template <typename T>
Expression<T>::Expression(
    char op,
    Expression<T>&& ternary,
    Expression<T>&& left,
    Expression<T>&& right)
    : simplified_value_(-1),
      bin_operator_node_cache_(nullptr),
      operator_node_cache_(std::make_unique<OperatorNode<T>>(
          op,
          std::move(ternary),
          std::move(left),
          std::move(right))) {
  CHECK(op == '?') << "'?' is the only allowed ternary operator at this time";
}

template <typename T>
Expression<T>::Expression()
    : simplified_value_(-1),
      bin_operator_node_cache_(nullptr),
      operator_node_cache_(nullptr) {}

template <typename T>
int Expression<T>::GetOperatorIndex(const std::string& expr) const {
  int exp_operator_index = -1;
  int mul_div_index = -1;
  int size = CheckedNumericCast<uint64_t, int>(expr.size());
  for (int i = size - 1; i >= 0; i--) {
    if (expr[i] == '+' || expr[i] == '-') {
      return i;
    }
    if (expr[i] == '*' || expr[i] == '/') {
      mul_div_index = i;
    } else if (expr[i] == '^') {
      exp_operator_index = i;
    }
  }
  return (mul_div_index != -1) ? mul_div_index : exp_operator_index;
}

template <typename T>
void Expression<T>::Parse(const std::string& expr) {
  int op_index = GetOperatorIndex(expr);
  if (op_index != -1) {
    bin_operator_node_cache_ = std::make_unique<BinOperatorNode<T>>(
        expr.substr(0, op_index),
        expr[op_index],
        expr.substr(op_index + 1));
  } else {
    simplified_value_ = GetSimplifiedVal(expr);
  }
}

template <typename T>
T Expression<T>::GetSimplifiedVal(const std::string& expr) const {
  return expr.empty() ? 0 : std::stoi(expr);
}

template <>
double Expression<double>::GetSimplifiedVal(const std::string& expr) const {
  return expr.empty() ? 0.0 : std::stod(expr);
}

template <typename T>
void Expression<T>::PrintAsTree(int indent) const {
  if (operator_node_cache_ != nullptr) {
    operator_node_cache_->PrintAsTree(indent);
  } else {
    std::cout << std::string(indent, kSeparator) << simplified_value_
              << std::endl;
  }
}

template <typename T>
void Expression<T>::ToStream(std::ostream& out) const {
  if (bin_operator_node_cache_ != nullptr) {
    bin_operator_node_cache_->ToStream(out);
  } else if (operator_node_cache_ == nullptr) {
    out << kConstantStr << " " << simplified_value_ << kSeparator;
  } else {
    operator_node_cache_->ToStream(out);
  }
}

template class Expression<int>;
template class Expression<double>;

}  // namespace code_experiments