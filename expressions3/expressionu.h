#ifndef EXPERIMENTS_EXPRESSIONS3_EXPRESSION_H
#define EXPERIMENTS_EXPRESSIONS3_EXPRESSION_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// NOTE: This is a temporary inclusion
#include "../expressions2/consts.h"

namespace code_experiments {

class ExpressionU {
public:
  void ReadOperands(std::istream& in, const std::string& specifier) {
    int num_operands = 2;
    if (specifier == kTOperatorStr) {
      op_ = "?";
      num_operands = 3;
    } else if (specifier == kFxOperatorStr) {
      in >> op_;
      in >> num_operands;
    } else {
      in >> op_;
    }
    for (int i = 0; i < num_operands; i++) {
      operands_.emplace_back(std::make_unique<ExpressionU>(in));
    }
  }

  explicit ExpressionU(std::istream& in) {
    std::string specifier;
    in >> specifier;
    if (specifier == kConstantStr) {
      in >> specifier;
      simplified_value_ = std::stoi(specifier);
      return;
    } else {
      ReadOperands(in, specifier);
    }
  }

  ExpressionU& operator=(ExpressionU&& from) noexcept {
    simplified_value_ = from.simplified_value_;
    op_ = std::move(from.op_);
    operands_ = std::move(from.operands_);
    return *this;
  }

  ExpressionU(ExpressionU&& from) noexcept
      : simplified_value_(from.simplified_value_),
        op_(std::move(from.op_)),
        operands_(std::move(from.operands_)) {}
  ExpressionU() {}
  ExpressionU(ExpressionU&& left, char op, ExpressionU&& right) : op_(1, op) {
    operands_.emplace_back(std::make_unique<ExpressionU>(std::move(left)));
    operands_.emplace_back(std::make_unique<ExpressionU>(std::move(right)));
  }
  ExpressionU(
      char op,
      ExpressionU&& ternary,
      ExpressionU&& left,
      ExpressionU&& right)
      : op_(1, op) {
    operands_.emplace_back(std::make_unique<ExpressionU>(std::move(ternary)));
    operands_.emplace_back(std::make_unique<ExpressionU>(std::move(left)));
    operands_.emplace_back(std::make_unique<ExpressionU>(std::move(right)));
  }
  ExpressionU(int constant) : simplified_value_(constant) {}
  ExpressionU(const ExpressionU& from) = delete;
  ~ExpressionU() = default;
  ExpressionU& operator=(ExpressionU& from) = delete;

  // int EvalAux() const;
  int Eval() const {
    if (operands_.empty()) {
      return simplified_value_;
    }
    int left = operands_[0]->Eval();
    int right = operands_[1]->Eval();
    switch (op_[0]) {
      case '+':
        return left + right;
      case '-':
        return left - right;
      case '*':
        return left * right;
      case '/':
        return left / right;
      case '^':
        return std::pow(left, right);
      default:
        return EvalAux();
    }
  }

  int EvalAux() const {
    // NOTE: Below storage can be removed in favor of a lambda that does Eval()
    // during comparison likely at the cost of at least one extra Eval.
    std::vector<int> vals;
    for (auto& operand : operands_) {
      vals.emplace_back(operand->Eval());
    }
    if (op_[0] == '?') {
      return vals[0] ? vals[1] : vals[2];
    } else if (op_ == "min") {  // NOLINT(readability-else-after-return)
      return *std::min_element(vals.begin(), vals.end());
    } else if (op_ == "max") {  // NOLINT(readability-else-after-return)
      return *std::max_element(vals.begin(), vals.end());
    } else {  // NOLINT(readability-else-after-return)
      CHECK(false) << "Unexpected function: " << op_;
    }
  }

  std::string ToStringWithParenAux() const {
    std::string out = op_ + "(";
    for (int i = 0; i < operands_.size(); i++) {
      out.append(i == 0 ? "" : ",");
      out.append(operands_[i]->ToStringWithParen());
    }
    out.append(")");
    return out;
  }

  std::string ToStringWithParen() const {
    if (operands_.empty()) {
      return "(" + std::to_string(simplified_value_) + ")";
    }
    if (op_.size() > 1) {
      return ToStringWithParenAux();
    } else if (op_[0] == '?') {  // NOLINT(readability-else-after-return)
      return "(" + operands_[0]->ToStringWithParen() + "?" +
             operands_[1]->ToStringWithParen() + ":" +
             operands_[2]->ToStringWithParen() + ")";
    } else {
      return "(" + operands_[0]->ToStringWithParen() + op_[0] +
             operands_[1]->ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream& out) const {
    if (operands_.empty()) {
      out << kConstantStr << " " << simplified_value_ << kSeparator;
      return;
    }
    if (op_.size() > 1) {
      out << kFxOperatorStr << kSeparator;
      out << op_;
      out << operands_.size();
    } else if (op_[0] == '?') {
      out << kTOperatorStr << kSeparator;
    } else {
      out << kBOperatorStr << kSeparator << op_[0] << kSeparator;
    }
    for (const auto& operand : operands_) {
      operand->ToStream(out);
    }
  }

private:
  int simplified_value_;
  std::string op_;
  std::vector<std::unique_ptr<ExpressionU>> operands_;
};

}  // namespace code_experiments

#endif