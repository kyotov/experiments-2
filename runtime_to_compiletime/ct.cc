#include <gflags/gflags.h>
#include <glog/logging.h>

#include <cmath>
#include <memory>
#include <sstream>
#include <string>

#include "../expressions2/expression.h"

DEFINE_string(expression, "", "Specify expression");  // NOLINT

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  code_experiments::Expression<double> expr(FLAGS_expression);
  LOG(INFO) << FLAGS_expression << " = " << expr.Eval();
  expr.PrintAsTree(0);
  LOG(INFO) << expr.ToStringWithParen();

  return 0;
}
