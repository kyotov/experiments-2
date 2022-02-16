#ifndef EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H
#define EXPERIMENTS_EXPRESSIONS2_OPERATOR_NODE_H

#include <glog/logging.h>

#include <algorithm>
#include <cmath>

#include "consts.h"
#include "expression.h"

namespace code_experiments {

template <typename T>
class Expression;

template <typename T>
class OperatorNode {
public:
  OperatorNode(const std::string &left, char op, const std::string &right);
  inline OperatorNode(std::istream &in, const std::string &specifier) {
    int num_operands = 2;
    if (specifier == kTOperatorStr) {
      num_operands = 3;
      func_ = "?";
    } else if (specifier == kFxOperatorStr) {
      in >> func_;
      in >> num_operands;
    } else {
      in >> func_;
    }
    for (int i = 0; i < num_operands; i++) {
      operands_.emplace_back(in);
    }
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(
      char op,
      Expression<T> &&ternary,
      Expression<T> &&left,
      Expression<T> &&right);
  OperatorNode(const OperatorNode<T> &from);
  OperatorNode(OperatorNode<T> &&from) noexcept = delete;
  ~OperatorNode() = default;
  OperatorNode<T> &operator=(OperatorNode<T> &from) = delete;
  OperatorNode<T> &operator=(OperatorNode<T> &&from) noexcept = delete;

  [[nodiscard]] T EvalFunc() const {
    // auto comparison = [](const Expression<T> &e1, const Expression<T> &e2) {
    //   return e1.Eval() < e2.Eval();
    // };
    if (func_ == "min") {
      T min = std::numeric_limits<int>::max();
      for (const auto &operand : operands_) {
        T val = operand.Eval();
        if (val < min) {
          min = val;
        }
      }
      return min;
      // return std::min(operands_, comparison);
    } else if (func_ == "max") {
      T max = std::numeric_limits<int>::min();
      for (const auto &operand : operands_) {
        T val = operand.Eval();
        if (val > max) {
          max = val;
        }
      }
      return max;
      // return std::max(operands_, comparison);
    } else {
      CHECK(false) << "Unexpected function: " << func_;
    }
  }

  [[nodiscard]] inline T Eval() const {
    // NOTE: The below check will need to change if an operator can be the first
    // character in a function name.
    switch (func_[0]) {
      case '+':
        return operands_[0].Eval() + operands_[1].Eval();
      case '-':
        return operands_[0].Eval() - operands_[1].Eval();
      case '*':
        return operands_[0].Eval() * operands_[1].Eval();
      case '/':
        return operands_[0].Eval() / operands_[1].Eval();
      case '^':
        return std::pow(operands_[0].Eval(), operands_[1].Eval());
      case '?':
        return operands_[0].Eval() ? operands_[1].Eval() : operands_[2].Eval();
      default:
        return EvalFunc();
    }
  }

  void PrintAsTree(int indent);

  [[nodiscard]] inline std::string FuncToStringWithParen() {
    std::string out = func_ + "(";
    // TODO(ashish): add consts
    for (int i = 0; i < operands_.size(); i++) {
      out.append(i == 0 ? "" : ",");
      out.append(operands_[i].ToStringWithParen());
    }
    out.append(")");
    return out;
  }

  [[nodiscard]] inline std::string ToStringWithParen() {
    if (func_.size() > 1) {
      return FuncToStringWithParen();
    } else if (func_[0] == '?') {
      return "(" + operands_[0].ToStringWithParen() + "?" +
             operands_[1].ToStringWithParen() + ":" +
             operands_[2].ToStringWithParen() + ")";
    } else {
      return "(" + operands_[0].ToStringWithParen() + func_ +
             operands_[1].ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream &out);

private:
  std::string func_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif