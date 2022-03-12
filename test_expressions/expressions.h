#ifndef TEST_EXPRESSIONS_H
#define TEST_EXPRESSIONS_H

#include <sstream>

#include "../expressions2/utils.h"
#include "../ky_expressions/expressions.h"
#include "../ky_expressions/expressions_dd.h"
#include "../ca_expressions/expressions.h"

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
  std::stringstream s{};
  D::Save(e, s);
  return s.str();
}

// NOTE: this should probably be generic enough to work for both of us...
template <typename D>
typename D::Expr LoadFromString(const std::string &i) {
  std::stringstream s{i};
  return D::Load(s);
}

#endif
