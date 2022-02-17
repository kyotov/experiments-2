#include "operator_node.h"

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
OperatorNode<T>::OperatorNode(
    Expression<T> &&left,
    char op,
    Expression<T> &&right)
    : left_(std::move(left)),
      operator_(op),
      right_(std::move(right)) {}

template <typename T>
OperatorNode<T>::OperatorNode(
    char op,
    Expression<T> &&ternary,  // NOLINT
    Expression<T> &&left,
    Expression<T> &&right)
    : operator_(op),
      left_(std::move(left)),
      right_(std::move(right)) {
  operands_.emplace_back(std::move(ternary));
}

template <typename T>
void OperatorNode<T>::PrintAsTree(int indent) {
  std::cout << std::string(indent, kSeparator) << operator_ << std::endl;
  left_.PrintAsTree(indent + 1);
  right_.PrintAsTree(indent + 1);
}

template <typename T>
[[nodiscard]] T OperatorNode<T>::EvalFunc() const {
  // NOTE: Below storage can be removed in favor of a lambda that does Eval()
  // during comparison likely at the cost of at least one extra Eval.
  std::vector<T> vals;
  for (auto &operand : operands_) {
    vals.emplace_back(operand.Eval());
  }
  if (func_ == "min") {
    return *std::min_element(vals.begin(), vals.end());
  } else if (func_ == "max") {  // NOLINT(readability-else-after-return)
    return *std::max_element(vals.begin(), vals.end());
  } else {  // NOLINT(readability-else-after-return)
    CHECK(false) << "Unexpected function: " << func_;
  }
}

template <typename T>
void OperatorNode<T>::ToStream(std::ostream &out) {
  if (operator_ == '?') {
    out << kTOperatorStr << kSeparator;
    operands_[0].ToStream(out);
  } else {
    out << kBOperatorStr << kSeparator << operator_ << kSeparator;
  }
  left_.ToStream(out);
  right_.ToStream(out);
}

template class OperatorNode<int>;
template class OperatorNode<double>;

}  // namespace code_experiments