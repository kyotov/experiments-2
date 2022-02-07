#include "expression.h"

#include <glog/logging.h>

#include "../cast_utils/cast_utils.h"
#include "consts.h"

namespace code_experiments {

template <typename T>
Expression<T>::Expression(const std::string& expr)
    : simplified_value_(-1),
      operator_node_cache_(nullptr) {
  Parse(expr);
}

template <typename T>
T Expression<T>::Eval() {
  if (operator_node_cache_ != nullptr) {
    return operator_node_cache_->Eval();
  }
  return simplified_value_;
}

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
    operator_node_cache_ = std::make_unique<OperatorNode<T>>(
        expr.substr(0, op_index),
        expr[op_index],
        expr.substr(op_index + 1));
    return;
  }
  simplified_value_ = GetSimplifiedVal(expr);
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
void Expression<T>::PrintAsTree(int indent) {
  if (operator_node_cache_ != nullptr) {
    operator_node_cache_->PrintAsTree(indent);
  } else {
    std::cout << std::string(indent, kSeparator) << simplified_value_
              << std::endl;
  }
}

template <typename T>
std::string Expression<T>::ToStringWithParen() {
  if (operator_node_cache_ != nullptr) {
    return operator_node_cache_->ToStringWithParen();
  }
  return "(" + std::to_string(simplified_value_) + ")";
}

template <typename T>
void Expression<T>::ToStream(std::ostream& out) {
  if (operator_node_cache_ != nullptr) {
    operator_node_cache_->ToStream(out);
  } else {
    out << kConstantStr << " " << simplified_value_ << kSeparator;
  }
}

template class Expression<int>;
template class Expression<double>;

}  // namespace code_experiments