#include <gtest/gtest.h>

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
#ifdef ASHISH_POPULATED_DRIVER

// Ashish's Driver (no dynamic dispatch)
class AtExpressionsDriver {
  using Expr = ...

      public : int
               compute(Expr e) { /* ... */
  }
  void Save(Expr e, std::ostream &s) { /* ... */
  }
  Expr Load(std::istream &s) { /* ... */
  }
  Expr ConstantExpression(int value) { /* ... */
  }
  Expr BinaryOperatorExpression(char op, Expr l, Expr r) { /* ... */
  }
}
#endif

// Kamen's [Single] Dynamic Dispatch Driver
class KyExpressionsDriver {
public:
  using Expr = std::unique_ptr<ky_expr::Expression>;

  static int Compute(Expr &e) { return e->Compute(); }

  static std::string AsString(Expr &e) { return e->AsString(); }

  static void Save(Expr &e, std::ostream &s) { e->Save(s); }

  static Expr Load(std::istream &s) { return ky_expr::Expression::Load(s); }

  static Expr ConstantExpression(int value) {
    return std::make_unique<ky_expr::ConstantExpression>(value);
  }

  static Expr BinaryOperatorExpression(char op, Expr l, Expr r) {
    return std::make_unique<ky_expr::BinaryOperatorExpression>(
        op,
        std::move(l),
        std::move(r));
  }
};

// Kamen's Double Dynamic Dispatch Driver
class KyExpressionsDriverDD {
public:
  using Expr = std::unique_ptr<ky_expr::Expression>;

  static int Compute(Expr &e) { return ky_expr::Expressions::Compute(e); }

  static std::string AsString(Expr &e) {
    return ky_expr::Expressions::AsString(e);
  }

  static void Save(Expr &e, std::ostream &s) {
    ky_expr::Expressions::Save(e, s);
  }

  static Expr Load(std::istream &s) { return ky_expr::Expression::Load(s); }

  static Expr ConstantExpression(int value) {
    return std::make_unique<ky_expr::ConstantExpression>(value);
  }

  static Expr BinaryOperatorExpression(char op, Expr l, Expr r) {
    return std::make_unique<ky_expr::BinaryOperatorExpression>(
        op,
        std::move(l),
        std::move(r));
  }
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
void test1() {
  auto e = D::BOp('+', D::C(2), D::C(2));

  ASSERT_EQ(D::Compute(e), 4);
  ASSERT_EQ(D::AsString(e), "((2)+(2))");

  std::string result = SaveToString<D>(e);
  ASSERT_EQ(result, "BOp + C 2 C 2 ");

  typename D::Expr e2 = LoadFromString<D>("BOp + C 2 C 2 ");
  ASSERT_EQ(D::AsString(e2), "((2)+(2))");
}

#ifdef ASHISH_POPULATED_DRIVER
TEST(Expressions, AtExpressions) { test1<AtExpressionsDriver>(); }
#endif

TEST(Expressions, KyExpressions) { test1<KyExpressionsDriver>(); }

TEST(Expressions, KyExpressionsDD) { test1<KyExpressionsDriverDD>(); }
