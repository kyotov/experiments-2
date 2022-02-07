#include "expressions.h"

#include <map>
#include <sstream>

namespace ky_expr {

//--- Constant ---

ConstantExpression::ConstantExpression(int value) : value_(value) {}

int ConstantExpression::Compute() { return value_; }

std::string ConstantExpression::AsString() {
  return (std::stringstream() << "(" << value_ << ")").str();
}

std::unique_ptr<Expression> ConstantExpression::Load(std::istream &s) {
  int value;
  s >> value;
  return std::make_unique<ConstantExpression>(value);
}

void ConstantExpression::Save(std::ostream &s) { s << "C " << value_ << " "; }

//--- BinaryOperatorExpression ---

BinaryOperatorExpression::BinaryOperatorExpression(
    char op,
    std::unique_ptr<Expression> l,
    std::unique_ptr<Expression> r)
    : op_(op),
      l_(std::move(l)),
      r_(std::move(r)) {}

int BinaryOperatorExpression::Compute() {
  switch (op_) {
    case '+':
      return l_->Compute() + r_->Compute();
    case '-':
      return l_->Compute() - r_->Compute();
    case '*':
      return l_->Compute() * r_->Compute();
    case '/':
      return l_->Compute() / r_->Compute();
    case '^':
      return l_->Compute() ^ r_->Compute();
    default:
      abort();
  }
}

std::string BinaryOperatorExpression::AsString() {
  return (std::stringstream()
          << "(" << l_->AsString() << op_ << r_->AsString() << ")")
      .str();
}

std::unique_ptr<Expression> BinaryOperatorExpression::Load(std::istream &s) {
  char o;
  s >> o;
  return std::make_unique<BinaryOperatorExpression>(
      o,
      std::move(Expression::Load(s)),
      std::move(Expression::Load(s)));
}

void BinaryOperatorExpression::Save(std::ostream &s) {
    s << "BOp " << op_ << " ";
    l_->Save(s);
    r_->Save(s);
}

//--- Expression ---

std::unique_ptr<Expression> Expression::Load(std::istream &s) {
  using Loader = std::unique_ptr<Expression> (*)(std::istream &);
  static std::map<std::string, Loader> loaders;

  static bool initialized = false;
  if (!initialized) {
    loaders["C"] = ConstantExpression::Load;
    loaders["BOp"] = BinaryOperatorExpression::Load;
  }

  std::string key;
  s >> key;
  auto i = loaders.find(key);
  if (i == loaders.end()) {
    abort();
  }
  return i->second(s);
}

}  // namespace ky_expr
