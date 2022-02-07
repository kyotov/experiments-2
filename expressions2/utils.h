#ifndef EXPERIMENTS_EXPRESSIONS2_UTILS_H
#define EXPERIMENTS_EXPRESSIONS2_UTILS_H

#include <string>

#include "expression.h"

namespace code_experiments {

using Expr = Expression<int>;

Expr ParseInput(const std::string& input) { return Expression<int>(input); }

}  // namespace code_experiments

#endif