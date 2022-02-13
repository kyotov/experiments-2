#include <gtest/gtest.h>

#include "../expressions2/utils.h"
#include "../ky_expressions/expressions.h"
#include "../ky_expressions/expressions_dd.h"

// TODO(ashish): see if you can easily populate the folloing driver
//               this will give us common interface we can target.
//
//               In theory we can define a concept that both our
//               implementations satisfy, but it is an overkill for now.
//
//               You can see my implementation of the driver right under.
//               If you want to change something reasonable about the
//               driver interface, let me know and I will adjust mine.
//

// Ashish's Driver (no dynamic dispatch)
class AtExpressionsDriver {
public:
  using Expr = code_experiments::Expr;

  static int Compute(Expr &e) { return e.Eval(); }
  static std::string AsString(Expr &e) { return e.ToStringWithParen(); }
  static void Save(Expr &e, std::ostream &s) { e.ToStream(s); }
  static Expr Load(std::istream &s) { return code_experiments::ParseInput(s); }
  static Expr ConstantExpression(int value) {
    return code_experiments::Expr(value);
  }
  static Expr BinaryOperatorExpression(char op, Expr l, Expr r) {
    return code_experiments::Expr(std::move(l), op, std::move(r));
  }
  static Expr TernaryOperatorExpression(Expr c, Expr t, Expr f) {
    return code_experiments::Expr(
        '?',
        std::move(c),
        std::move(t),
        std::move(f));
  }
};

class KyExpressionsCommon {
public:
  using Expr = std::unique_ptr<ky_expr::Expression>;

  static Expr ConstantExpression(int value) {
    return std::make_unique<ky_expr::ConstantExpression>(value);
  }

  static Expr BinaryOperatorExpression(char op, Expr l, Expr r) {
    return std::make_unique<ky_expr::BinaryOperatorExpression>(
        op,
        std::move(l),
        std::move(r));
  }

  static Expr TernaryOperatorExpression(Expr c, Expr t, Expr f) {
    return std::make_unique<ky_expr::TernaryOperatorExpression>(
        std::move(c),
        std::move(t),
        std::move(f));
  }
};

// Kamen's [Single] Dynamic Dispatch Driver
class KyExpressionsDriver : public KyExpressionsCommon {
public:
  static int Compute(Expr &e) { return e->Compute(); }

  static std::string AsString(Expr &e) { return e->AsString(); }

  static void Save(Expr &e, std::ostream &s) { e->Save(s); }

  static Expr Load(std::istream &s) { return ky_expr::Expression::Load(s); }
};

// Kamen's Double Dynamic Dispatch Driver
class KyExpressionsDriverDD : public KyExpressionsCommon {
public:
  static int Compute(Expr &e) { return ky_expr::Expressions::Compute(e); }

  static std::string AsString(Expr &e) {
    return ky_expr::Expressions::AsString(e);
  }

  static void Save(Expr &e, std::ostream &s) {
    ky_expr::Expressions::Save(e, s);
  }

  static Expr Load(std::istream &s) { return ky_expr::Expression::Load(s); }
};

// NOTE: these shortcuts work for me... we might need to massage them for you.
#define C ConstantExpression
#define BOp BinaryOperatorExpression

// NOTE: this should probably be generic enough to work for both of us...
template <typename D>
std::string SaveToString(typename D::Expr &e) {
  std::stringstream s;
  D::Save(e, s);
  return s.str();
}

// NOTE: this should probably be generic enough to work for both of us...
template <typename D>
typename D::Expr LoadFromString(const std::string &i) {
  std::stringstream s{i};
  return D::Load(s);
}

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
}

template <typename D>
void RunTests() {
  Test1<D>();
  Test2<D>();
  Test3<D>();
  Test4<D>();
}

TEST(Expressions, AtExpressions) { RunTests<AtExpressionsDriver>(); }
// TODO(kamen): Uncomment the below when ternary logic updated.
// TEST(Expressions, KyExpressions) { RunTests<KyExpressionsDriver>(); }
// TEST(Expressions, KyExpressionsDD) { RunTests<KyExpressionsDriverDD>(); }

TEST(Expressions2, AtExpressions) { Test3<AtExpressionsDriver>(); }
// TEST(Expressions2, KyExpressions) { Test3<KyExpressionsDriver>(); }
// TEST(Expressions2, KyExpressionsDD) { Test3<KyExpressionsDriverDD>(); }
