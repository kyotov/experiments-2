#include "primes.h"

#include <cmath>

namespace code_experiments {

static bool IsPrime(int num, const std::vector<int> &smaller_primes) {
  int max_divisor = static_cast<int>(sqrt(num));
  for (auto divisor : smaller_primes) {
    if (divisor > max_divisor) {
      break;
    }
    if (num % divisor == 0) {
      return false;
    }
  }
  return true;
}

void PrimeNumbers::Enlarge() {
  for (auto num = prime_numbers_.back() + 2;; num += 2) {
    if (IsPrime(num, prime_numbers_)) {
      prime_numbers_.push_back(num);
      return;
    }
  }
}

}  // namespace code_experiments