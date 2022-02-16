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
  inline OperatorNode(std::istream &in, const std::string &specifier)
      : operator_('@') {
    if (specifier == kTOperatorStr) {
      operands_.emplace_back((in));
      operator_ = '?';
    } else if (specifier == kFxOperatorStr) {
      in >> func_;
      int size;
      in >> size;
      for (int i = 0; i < size; i++) {
        operands_.emplace_back(in);
      }
      return;
    } else {
      in >> operator_;
    }
    left_ = std::move(Expression<T>(in));
    right_ = std::move(Expression<T>(in));
  }

  OperatorNode(Expression<T> &&left, char op, Expression<T> &&right);
  OperatorNode(
      char op,
      Expression<T> &&ternary,
      Expression<T> &&left,
      Expression<T> &&right);
  OperatorNode(const OperatorNode<T> &from) = delete;
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
    T left = left_.Eval();
    T right = right_.Eval();
    switch (operator_) {
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
      case '?':
        return operands_[0].Eval() ? left : right;
      case '@':
        return EvalFunc();
      default:
        LOG_ASSERT(false) << "Unexpected operator: " << operator_;
    }
  }

  void PrintAsTree(int indent);

  [[nodiscard]] inline std::string ToStringWithParen() {
    if (operator_ == '?') {
      return "(" + operands_[0].ToStringWithParen() + "?" +
             left_.ToStringWithParen() + ":" + right_.ToStringWithParen() + ")";
    } else if (operator_ == '@') {
      std::string out = func_ + "(";
      // TODO(ashish): add consts
      for (int i = 0; i < operands_.size(); i++) {
        out.append(i == 0 ? "" : ",");
        out.append(operands_[i].ToStringWithParen());
      }
      out.append(")");
      return out;
    } else {
      return "(" + left_.ToStringWithParen() + operator_ +
             right_.ToStringWithParen() + ")";
    }
  }

  void ToStream(std::ostream &out);

private:
  Expression<T> left_;
  char operator_;
  Expression<T> right_;
  std::string func_;
  std::vector<Expression<T>> operands_;
};

}  // namespace code_experiments

#endif