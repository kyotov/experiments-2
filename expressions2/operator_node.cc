#include "operator_node.h"

#include <glog/logging.h>

#include <cmath>

#include "consts.h"

namespace code_experiments {

template <typename T>
OperatorNode<T>::OperatorNode(
    const std::string &left,
    char op,
    const std::string &right)
    : left_(left),
      operator_(op),
      right_(right) {}

template <typename T>
OperatorNode<T>::OperatorNode(std::istream &in) : operator_('?') {
  in >> operator_;
  left_ = std::move(Expression<T>(in));
  right_ = std::move(Expression<T>(in));
}

template <typename T>
OperatorNode<T>::OperatorNode(
    Expression<T> &&left,
    char op,
    Expression<T> &&right)
    : left_(std::move(left)),
      operator_(op),
      right_(std::move(right)) {}

template <typename T>
OperatorNode<T>::OperatorNode(const OperatorNode<T> &from)
    : left_(from.left_),
      operator_(from.operator_),
      right_(from.right_) {}

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
  std::cout << std::string(indent, kSeparator) << operator_ << std::endl;
  left_.PrintAsTree(indent + 1);
  right_.PrintAsTree(indent + 1);
}

template <typename T>
std::string OperatorNode<T>::ToStringWithParen() {
  return "(" + left_.ToStringWithParen() + operator_ +
         right_.ToStringWithParen() + ")";
}

template <typename T>
void OperatorNode<T>::ToStream(std::ostream &out) {
  out << kBOperatorStr << " " << operator_ << kSeparator;
  left_.ToStream(out);
  right_.ToStream(out);
}

template class OperatorNode<int>;
template class OperatorNode<double>;

}  // namespace code_experiments