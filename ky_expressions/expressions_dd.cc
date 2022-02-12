#include "expressions_dd.h"

#include <sstream>

namespace ky_expr {

int Expressions::Compute(Expressions::Expr &e) {
  class V : public ExpressionVisitor {
  public:
    void Visit(ConstantExpression &e) { result_ = e.GetValue(); }

    void Visit(BinaryOperatorExpression &e) {
      e.GetLHS().Accept(*this);
      int l = result_;

      e.GetRHS().Accept(*this);
      int r = result_;

      switch (e.GetOp()) {
        case '+':
          result_ = l + r;
          break;
        case '-':
          result_ = l - r;
          break;
        case '*':
          result_ = l * r;
          break;
        case '/':
          result_ = l / r;
          break;
        case '^':
          result_ = l ^ r;
          break;
        default:
          abort();
      }
    }

    int result_;
  } v;

  e->Accept(v);
  return v.result_;
}

std::string Expressions::AsString(Expressions::Expr &e) {
  class V : public ExpressionVisitor {
  public:
    void Visit(ConstantExpression &e) {
      std::stringstream stream;
      stream << "(" << e.GetValue() << ")";
      result_ = stream.str();
    }

    void Visit(BinaryOperatorExpression &e) {
      e.GetLHS().Accept(*this);
      auto l = std::move(result_);
      e.GetRHS().Accept(*this);
      auto r = std::move(result_);
      std::stringstream stream;
      stream << "(" << l << e.GetOp() << r << ")";
      result_ = stream.str();
    }

    std::string result_;
  } v;

  e->Accept(v);
  return v.result_;
}

void Expressions::Save(Expressions::Expr &e, std::ostream &s) {
  class V : public ExpressionVisitor {
  public:
    V(std::ostream &s) : s_(s) {}

    // NOLINTNEXTLINE()
    void Visit(ConstantExpression &e) { s_ << "C " << e.GetValue() << " "; }

    void Visit(BinaryOperatorExpression &e) {
      // NOLINTNEXTLINE()
      s_ << "BOp " << e.GetOp() << " ";
      e.GetLHS().Accept(*this);
      e.GetRHS().Accept(*this);
    }

    std::ostream &s_;
  } v(s);

  e->Accept(v);
}

}  // namespace ky_expr
