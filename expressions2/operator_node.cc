#include "operator_node.h"

#include <glog/logging.h>

#include <cmath>

namespace code_experiments {

template <typename T>
T OperatorNode<T>::Eval() {
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
    default:
      LOG_ASSERT(false) << "Unexpected operator: " << operator_;
  }
}

template <typename T>
void OperatorNode<T>::PrintAsTree(int indent) {
  std::cout << std::string(indent, ' ') << operator_ << std::endl;
  left_.PrintAsTree(indent + 1);
  right_.PrintAsTree(indent + 1);
}

template <typename T>
std::string OperatorNode<T>::ToStringWithParen() {
  return "(" + left_.ToStringWithParen() + operator_ +
         right_.ToStringWithParen() + ")";
}

template class OperatorNode<int>;
template class OperatorNode<double>;

}  // namespace code_experiments