#include "bin_operator_node.h"

namespace code_experiments {

template <typename T>
BinOperatorNode<T>::BinOperatorNode(
    const std::string &left,
    char op,
    const std::string &right)
    : op_(op),
      left_(left),
      right_(right) {}

template <typename T>
BinOperatorNode<T>::BinOperatorNode(
    Expression<T> &&left,
    char op,
    Expression<T> &&right)
    : op_(op),
      left_(std::move(left)),
      right_(std::move(right)) {}

template <typename T>
void BinOperatorNode<T>::PrintAsTree(int indent) const {
  std::cout << std::string(indent, kSeparator) << op_ << std::endl;
  left_.PrintAsTree(indent + 1);
  right_.PrintAsTree(indent + 1);
}

template <typename T>
[[nodiscard]] std::string BinOperatorNode<T>::ToStringWithParen() const {
  return "(" + left_.ToStringWithParen() + op_ + right_.ToStringWithParen() +
         ")";
}

template <typename T>
void BinOperatorNode<T>::ToStream(std::ostream &out) const {
  out << kBOperatorStr << kSeparator << op_ << kSeparator;
  left_.ToStream(out);
  right_.ToStream(out);
}

template class BinOperatorNode<int>;
template class BinOperatorNode<double>;

}  // namespace code_experiments