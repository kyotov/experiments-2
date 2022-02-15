#include "utils.h"

namespace code_experiments {

Expr ParseInput(const std::string& input) { return Expression<int>(input); }
Expr ParseInput(std::istream& input) { return Expression<int>(input); }

}  // namespace code_experiments
