#ifndef EXPERIMENTS_EXPRESSIONS2_EXPRESSIONSV_H
#define EXPERIMENTS_EXPRESSIONS2_EXPRESSIONSV_H

#include <memory>
#include <string>
#include <variant>

#include "consts.h"

namespace code_experiments {

class Constant;
class BinaryOperator;

using ExprV =
    std::variant<std::unique_ptr<Constant>, std::unique_ptr<BinaryOperator>>;

ExprV CreateFromStream(std::istream& in);
inline int ExprEval(ExprV& expr);

class Constant {
public:
  explicit Constant(int value) : value_(value) {}

  static ExprV FromStream(std::istream& in) {
    int value;
    in >> value;
    return std::move(std::make_unique<Constant>(value));
  }

  int Eval() { return value_; }

private:
  int value_;
};

class BinaryOperator {
public:
  BinaryOperator(ExprV&& left, char op, ExprV&& right)
      : left_(std::move(left)),
        op_(op),
        right_(std::move(right)) {}

  static ExprV FromStream(std::istream& in) {
    char op;
    in >> op;
    ExprV left = CreateFromStream(in);
    ExprV right = CreateFromStream(in);
    return std::move(std::make_unique<BinaryOperator>(
        std::move(left),
        op,
        std::move(right)));
  }

  int Eval() {
    int left = ExprEval(left_);
    int right = ExprEval(right_);
    return left + right;
  }

private:
  ExprV left_;
  char op_;
  ExprV right_;
};

inline ExprV CreateFromStream(std::istream& in) {
  std::string specifier;
  in >> specifier;
  if (specifier == kBOperatorStr) {
    return BinaryOperator::FromStream(in);
  } else {
    // Note: Can check whether specifier is constant here
    // CHECK(specifier == kConstantStr) << "Unexpected: " << specifier;
    return Constant::FromStream(in);
  }
}

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

inline int ExprEval(ExprV& expr) {
  int result;
  auto eval = overloaded{
      [&](std::unique_ptr<Constant>& p) { result = p->Eval(); },
      [&](std::unique_ptr<BinaryOperator>& p) { result = p->Eval(); }};
  std::visit(eval, expr);
  return result;
}

}  // namespace code_experiments

#endif