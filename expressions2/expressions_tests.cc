#include <glog/logging.h>
#include <gtest/gtest.h>

#include <map>

#include "consts.h"
#include "expression.h"
#include "operator_node.h"
#include "utils.h"

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
        auto e = ParseInput(element.first);
        int value = e.Eval();
        LOG(INFO) << element.first << " = " << value;
        EXPECT_EQ(value, element.second);
      });
}

TEST_F(ExpressionsTests, BasicReadabilityCheck) {  // NOLINT
  std::string input("1+2");
  auto e = ParseInput(input);
  LOG(INFO) << "Result is: " << e.Eval();
  EXPECT_EQ(e.Eval(), 3);
}

TEST_F(ExpressionsTests, BasicDoubleCheck) {  // NOLINT
  std::string input("1.1+2*1.5");
  auto e = ParseInputD(input);
  LOG(INFO) << "Result is: " << e.Eval();
  EXPECT_EQ(e.Eval(), 4.1);
}

TEST_F(ExpressionsTests, BasicToStreamCheck) {  // NOLINT
  std::string input("1+2");
  auto e = ParseInput(input);
  std::stringstream out;
  LOG(INFO) << "Result is: " << e.Eval();
  e.ToStream(out);
  LOG(INFO) << out.str();
  EXPECT_EQ(
      kBOperatorStr + " + " + kConstantStr + " 1 " + kConstantStr + " 2 ",
      out.str());
}

TEST_F(ExpressionsTests, BasicFromStreamCheck) {  // NOLINT
  std::stringstream stream(
      kBOperatorStr + " + " + kConstantStr + " 5 " + kConstantStr + " 2");
  auto e = ParseInput(stream);
  LOG(INFO) << "Result is: " << e.Eval();
  e.PrintAsTree(0);
  EXPECT_EQ(7, e.Eval());
}

TEST_F(ExpressionsTests, BasicFromStreamDoubleCheck) {  // NOLINT
  std::stringstream stream(
      kBOperatorStr + " + " + kConstantStr + " 5.1 " + kConstantStr + " 2.2");
  auto e = ParseInputD(stream);
  LOG(INFO) << "Result is: " << e.Eval();
  e.PrintAsTree(0);
  EXPECT_EQ(7.3, e.Eval());
}

TEST_F(ExpressionsTests, ToFromStreamCheck) {  // NOLINT
  std::string input("1+2*3-4");
  const int result = 3;
  auto e = ParseInput(input);
  std::stringstream stream;
  LOG(INFO) << "Result is: " << e.Eval();
  EXPECT_EQ(result, e.Eval());
  e.ToStream(stream);
  LOG(INFO) << stream.str();
  EXPECT_EQ(
      kBOperatorStr + " - " + kBOperatorStr + " + " + kConstantStr + " 1 " +
          kBOperatorStr + " * " + kConstantStr + " 2 " + kConstantStr + " 3 " +
          kConstantStr + " 4 ",
      stream.str());
  stream.seekg(0);
  auto e2 = ParseInput(stream);
  LOG(INFO) << "Result on read value: " << e2.Eval();
  e2.PrintAsTree(0);
  EXPECT_EQ(result, e2.Eval());
}

}  // namespace code_experiments