#include "utils.h"

namespace code_experiments {

Expr ParseInput(const std::string& input) { return Expression<int>(input); }
Expr ParseInput(std::istream& input) { return Expression<int>(input); }

ExprD ParseInputD(const std::string& input) {
  return Expression<double>(input);
}

ExprD ParseInputD(std::istream& input) { return Expression<double>(input); }

}  // namespace code_experiments
