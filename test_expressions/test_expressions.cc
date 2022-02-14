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
  auto e1 = D::BOp('+', D::C(2), D::C(2));
  ASSERT_EQ(D::Compute(e1), 4);
  auto e2 = D::BOp('+', std::move(e1), D::C(2));
  ASSERT_EQ(D::Compute(e2), 6);
}

template <typename D>
void Test3() {
  typename D::Expr e1 = LoadFromString<D>("TOp BOp + C 1 C 1 C 101 C 202 ");
  EXPECT_EQ(D::AsString(e1), "(((1)+(1))?(101):(202))");
  EXPECT_EQ(D::Compute(e1), 101);

  typename D::Expr e2 = LoadFromString<D>("TOp BOp - C 1 C 1 C 101 C 202 ");
  EXPECT_EQ(D::AsString(e2), "(((1)-(1))?(101):(202))");
  EXPECT_EQ(D::Compute(e2), 202);
}

template <typename D>
void Test4() {
  auto ct = D::C(1);
  auto e1 = D::TernaryOperatorExpression(std::move(ct), D::C(4), D::C(6));
  ASSERT_EQ(D::Compute(e1), 4);
  auto cf = D::C(0);
  auto e2 = D::TernaryOperatorExpression(std::move(cf), D::C(4), D::C(6));
  ASSERT_EQ(D::Compute(e2), 6);
  std::stringstream stream;
  D::Save(e2, stream);
  stream.seekg(0);
  auto e3 = D::Load(stream);
  ASSERT_EQ(D::Compute(e3), 6);
}

template <typename D>
void Test5() {
  std::string s = "BOp + C 1 C 1 ";
  for (int i = 0; i < 20; i++) {
    s = "BOp + " + s + s;
  }
  typename D::Expr e = LoadFromString<D>(s);
  ASSERT_EQ(D::Compute(e), 2097152);
}

template <typename D>
void RunTests() {
  Test1<D>();
  Test2<D>();
  Test3<D>();
  Test4<D>();
  Test5<D>();
}

TEST(Expressions, AtExpressions) { RunTests<AtExpressionsDriver>(); }
// TODO(kamen): Uncomment the below when ternary logic updated.
// TEST(Expressions, KyExpressions) { RunTests<KyExpressionsDriver>(); }
// TEST(Expressions, KyExpressionsDD) { RunTests<KyExpressionsDriverDD>(); }

TEST(Expressions2, AtExpressions) { Test3<AtExpressionsDriver>(); }
// TEST(Expressions2, KyExpressions) { Test3<KyExpressionsDriver>(); }
// TEST(Expressions2, KyExpressionsDD) { Test3<KyExpressionsDriverDD>(); }
