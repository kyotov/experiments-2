#include <gtest/gtest.h>

#include "expressions.h"

template <typename D>
void Test1() {
  auto e = D::BOp('+', D::C(2), D::C(2));

  ASSERT_EQ(D::Compute(e), 4);
  ASSERT_EQ(D::AsString(e), "((2)+(2))");

  std::string result = SaveToString<D>(e);
  ASSERT_EQ(result, "BOp + C 2 C 2 ");

  typename D::Expr e2 = LoadFromString<D>("BOp + C 2 C 2 ");
  ASSERT_EQ(D::AsString(e2), "((2)+(2))");
}

template <typename D>
void Test2() {
  auto c1 = D::C(2);
  auto e1 = D::BOp('+', c1, c1);
  ASSERT_EQ(D::Compute(e1), 4);
  auto e2 = D::BOp('+', e1, c1);
  ASSERT_EQ(D::Compute(e2), 6);
}

template <typename D>
void RunTests() {
  Test1<D>();
  // Test2<D>();
}

TEST(Expressions, AtExpressions) { RunTests<AtExpressionsDriver>(); }
TEST(ExpressionsTest2, AtExpressions) { Test2<AtExpressionsDriver>(); }
TEST(Expressions, KyExpressions) { RunTests<KyExpressionsDriver>(); }
TEST(Expressions, KyExpressionsDD) { RunTests<KyExpressionsDriverDD>(); }
