#include "expression.h"

#include <glog/logging.h>

namespace code_experiments {

template <typename T>
int Expression<T>::GetOperatorIndex() const {
  int exp_operator_index = -1;
  int mul_div_index = -1;
  for (int i = expr_.size() - 1; i >= 0; i--) {
    if (expr_[i] == '+' || expr_[i] == '-') {
      return i;
    }
    if (expr_[i] == '*' || expr_[i] == '/') {
      mul_div_index = i;
    } else if (expr_[i] == '^') {
      exp_operator_index = i;
    }
  }
  return (mul_div_index != -1) ? mul_div_index : exp_operator_index;
}

template <typename T>
T Expression<T>::ParseAndEvaluate() {
  int op_index = GetOperatorIndex();
  if (op_index != -1) {
    operator_node_cache_ = std::make_unique<OperatorNode<T>>(
        expr_.substr(0, op_index),
        expr_[op_index],
        expr_.substr(op_index + 1));
    return operator_node_cache_->Eval();
  }
  return expr_.empty() ? 0 : std::stoi(expr_);
}

template <typename T>
void Expression<T>::PrintAsTree(int indent) {
  if (operator_node_cache_ != nullptr) {
    operator_node_cache_->PrintAsTree(indent);
  } else {
    std::cout << std::string(indent, ' ') << expr_ << std::endl;
  }
}

template <typename T>
std::string Expression<T>::ToStringWithParen() {
  if (operator_node_cache_ != nullptr) {
    return operator_node_cache_->ToStringWithParen();
  }
  return "(" + expr_ + ")";
}

template class Expression<int>;

}  // namespace code_experiments