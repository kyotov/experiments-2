#include "operator_node.h"

namespace code_experiments {

template <typename T>
OperatorNode<T>::OperatorNode(
    const std::string &left,
    char op,
    const std::string &right)
    : op_(1, op) {
  operands_.emplace_back(left);
  operands_.emplace_back(right);
}

template <typename T>
OperatorNode<T>::OperatorNode(
    Expression<T> &&left,
    char op,
    Expression<T> &&right)
    : op_(1, op) {
  operands_.emplace_back(std::move(left));
  operands_.emplace_back(std::move(right));
}

template <typename T>
OperatorNode<T>::OperatorNode(
    char op,
    Expression<T> &&ternary,
    Expression<T> &&left,
    Expression<T> &&right)
    : op_(1, op) {
  operands_.emplace_back(std::move(ternary));
  operands_.emplace_back(std::move(left));
  operands_.emplace_back(std::move(right));
}

template <typename T>
void OperatorNode<T>::PrintAsTree(int indent) {
  std::cout << std::string(indent, kSeparator) << op_[0] << std::endl;
  operands_[0].PrintAsTree(indent + 1);
  operands_[1].PrintAsTree(indent + 1);
}

template <typename T>
[[nodiscard]] T OperatorNode<T>::EvalFunc() const {
  // NOTE: Below storage can be removed in favor of a lambda that does Eval()
  // during comparison likely at the cost of at least one extra Eval.
  std::vector<T> vals;
  for (auto &operand : operands_) {
    vals.emplace_back(operand.Eval());
  }
  if (op_[0] == '?') {
    return vals[0] ? vals[1] : vals[2];
  } else if (op_ == "min") {  // NOLINT(readability-else-after-return)
    return *std::min_element(vals.begin(), vals.end());
  } else if (op_ == "max") {  // NOLINT(readability-else-after-return)
    return *std::max_element(vals.begin(), vals.end());
  } else {  // NOLINT(readability-else-after-return)
    CHECK(false) << "Unexpected function: " << op_;
  }
}

template <typename T>
[[nodiscard]] std::string OperatorNode<T>::FuncToStringWithParen() {
  std::string out = op_ + "(";
  // TODO(ashish): add consts
  for (int i = 0; i < operands_.size(); i++) {
    out.append(i == 0 ? "" : ",");
    out.append(operands_[i].ToStringWithParen());
  }
  out.append(")");
  return out;
}

template <typename T>
[[nodiscard]] inline std::string OperatorNode<T>::ToStringWithParen() {
  if (op_.size() > 1) {
    return FuncToStringWithParen();
  } else if (op_[0] == '?') {  // NOLINT(readability-else-after-return)
    return "(" + operands_[0].ToStringWithParen() + "?" +
           operands_[1].ToStringWithParen() + ":" +
           operands_[2].ToStringWithParen() + ")";
  } else {
    return "(" + operands_[0].ToStringWithParen() + op_[0] +
           operands_[1].ToStringWithParen() + ")";
  }
}

template <typename T>
void OperatorNode<T>::ToStream(std::ostream &out) {
  if (op_.size() > 1) {
    out << kFxOperatorStr << kSeparator;
    out << op_;
    out << operands_.size();
    for (auto &operand : operands_) {
      operand.ToStream(out);
    }
    return;
  }
  if (op_[0] == '?') {
    out << kTOperatorStr << kSeparator;
  } else {
    out << kBOperatorStr << kSeparator << op_[0] << kSeparator;
  }
  for (auto &operand : operands_) {
    operand.ToStream(out);
  }
}

template class OperatorNode<int>;
template class OperatorNode<double>;

}  // namespace code_experiments