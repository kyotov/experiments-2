#ifndef EXPERIMENTS_IERATORS_PRIME_H
#define EXPERIMENTS_IERATORS_PRIME_H

#include <limits>
#include <vector>

namespace code_experiments {

class PrimeNumbers {
public:
  class Iterator : public virtual std::iterator<std::input_iterator_tag, int> {
  public:
    Iterator(PrimeNumbers &container, int index)
        : container_(container),
          index_(index) {}

    int operator*() { return container_.prime_numbers_[index_]; }

    Iterator &operator++() {
      index_++;
      Enlarge();
      return *this;
    }

    bool operator==(const Iterator &rhs) const {
      return index_ == rhs.index_ && container_ == rhs.container_;
    }

    bool operator!=(const Iterator &rhs) const { return !(*this == rhs); }

  private:
    void Enlarge() {
      // NOTE: This automatically grows the underlying container by design.
      while (index_ >= container_.prime_numbers_.size()) {
        container_.Enlarge();
      }
    }
    PrimeNumbers &container_;
    int index_;
  };

  explicit PrimeNumbers() : prime_numbers_{2, 3} {}
  // NOLINTNEXTLINE(readability-identifier-naming)
  Iterator begin() { return {*this, 0}; }
  // NOLINTNEXTLINE(readability-identifier-naming)
  Iterator end() { return {*this, std::numeric_limits<int>::max()}; }

  bool operator==(const PrimeNumbers &rhs) const {
    return prime_numbers_ == rhs.prime_numbers_;
  }

private:
  void Enlarge();

  std::vector<int> prime_numbers_;
};

}  // namespace code_experiments

#endif