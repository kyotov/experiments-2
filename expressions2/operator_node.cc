#include "operator_node.h"

namespace code_experiments {

template <typename T>
OperatorNode<T>::OperatorNode(
    const std::string &left,
    char op,
    const std::string &right)
    : func_(1, op) {
  operands_.emplace_back(left);
  operands_.emplace_back(right);
}

template <typename T>
OperatorNode<T>::OperatorNode(
    Expression<T> &&left,     // NOLINT
    char op,                  // NOLINT
    Expression<T> &&right) {  // NOLINT
  CHECK(false) << "Unexpected call";
}

template <typename T>
OperatorNode<T>::OperatorNode(
    char op,                  // NOLINT
    Expression<T> &&ternary,  // NOLINT
    Expression<T> &&left,     // NOLINT
    Expression<T> &&right) {  // NOLINT
  CHECK(false) << "Unexpected call";
}

template <typename T>
OperatorNode<T>::OperatorNode(const OperatorNode<T> &from) {  // NOLINT
  CHECK(false) << "Unexpected call";
}

template <typename T>
void OperatorNode<T>::PrintAsTree(int indent) {
  std::cout << std::string(indent, kSeparator) << func_ << std::endl;
  operands_[0].PrintAsTree(indent + 1);
  operands_[1].PrintAsTree(indent + 1);
}

template <typename T>
void OperatorNode<T>::ToStream(std::ostream &out) {
  if (func_.size() > 1) {
    out << kFxOperatorStr << kSeparator;
    out << func_;
    out << operands_.size();
  } else if (func_[0] == '?') {
    out << kTOperatorStr << kSeparator;
  } else {
    out << kBOperatorStr << kSeparator << func_[0] << kSeparator;
  }
  for (auto &operand : operands_) {
    operand.ToStream(out);
  }
}

template class OperatorNode<int>;
template class OperatorNode<double>;

}  // namespace code_experiments