#include <glog/logging.h>
#include <gtest/gtest.h>

#include <map>

#include "expression.h"
#include "operator_node.h"

namespace code_experiments {

class ExpressionsTests : public ::testing::Test {};

TEST_F(ExpressionsTests, BasicOperatorCheck) {  // NOLINT
  std::string left("1");
  std::string right("2");
  OperatorNode<int> node(left, '+', right);
  EXPECT_EQ(node.Eval(), 3);
}

TEST_F(ExpressionsTests, BasicExpressionsCheck) {  // NOLINT
  std::string expression("1+2");
  Expression<int> expr(expression);
  EXPECT_EQ(expr.Eval(), 3);
}

TEST_F(ExpressionsTests, BasicNoParensCheck) {  // NOLINT
  std::string expression("5+3*5");
  Expression<int> expr(expression);
  EXPECT_EQ(expr.Eval(), 20);
  expression = "5-3*5";
  Expression<int> expr2(expression);
  EXPECT_EQ(expr2.Eval(), -10);
}

TEST_F(ExpressionsTests, BasicBodmasCheck) {  // NOLINT
  std::map<std::string, int> expression_to_value = {
      {"5+3*5+1", 21},       // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"5+3*5-1", 19},       // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"2-3-1", -2},         // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"-3-1+2", -2},        // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"+3-1+2", 4},         // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      {"1 + 4/2 - 5/4", 2},  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  };
  std::for_each(
      expression_to_value.begin(),
      expression_to_value.end(),
      [](const std::pair<std::string, int>& element) {
        int value = Expression<int>(element.first).Eval();
        LOG(INFO) << element.first << " = " << value;
        EXPECT_EQ(value, element.second);
      });
}

TEST_F(ExpressionsTests, BasicReadabilityCheck) {  // NOLINT
  using Expr = Expression<int>;

  std::string input("1+2");
  auto e = Expr(input);
  LOG(INFO) << "Result is: " << e.Eval();

  EXPECT_EQ(e.Eval(), 3);
}

}  // namespace code_experiments