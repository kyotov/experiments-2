#include <glog/logging.h>

#include "primes.h"

int main() {
  LOG(INFO) << "This is an iterator example.";
  code_experiments::PrimeNumbers primes;
  const int n = 100'000;
  int i = 0;
  for (auto x : primes) {
    if (++i == n) {
      LOG(INFO) << n << "(th) prime number is: " << x;
      break;
    }
  }
  LOG(INFO) << "Done.";
  return 0;
}