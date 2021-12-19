#include <glog/logging.h>

// #include <boost/type_index.hpp>

#include "primes.h"

template <typename T>
class TD;

template <typename T>
void PrintType(const T &param) {  // NOLINT(misc-unused-parameters)
  LOG(INFO) << "Boost typeindex based type checks can be enable below";
  // using boost::typeindex::type_id_with_cvr;
  // LOG(INFO) << "T = " << type_id_with_cvr<T>().pretty_name();
  // LOG(INFO) << "T = " << type_id_with_cvr<decltype(param)>().pretty_name();
}

// NOTE: Uncommenting the TD<> statements below helps get the type through the
// compiler. That seems to be more accurate than the type_id_with_cvr().
void IntTypeInfoCheck() {
  int x = 20;  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  const int cx = 30;
  const int &crx = 40;  // NOLINT(cppcoreguidelines-avoid-magic-numbers)

  auto y = crx;
  LOG(INFO) << "Getting type for auto y = crx: ";
  PrintType(y);

  // TD<decltype(y)> yType;
}

void TypeInfoCheck() {
  code_experiments::PrimeNumbers primes;
  const auto &it = primes.begin();
  auto it2 = it;
  LOG(INFO) << "Getting type for iterator: ";
  PrintType(it);
  LOG(INFO) << "Getting type for its auto assigned iterator: ";
  PrintType(it2);
  LOG(INFO) << "Getting type for its incremented value: ";
  PrintType(++it2);
  const auto &it3 = it;
  LOG(INFO) << "Getting type for const auto & assigned iterator: ";
  PrintType(it3);
  // TD<decltype(it)> itType;
  // TD<decltype(it2)> it2Type;
  // TD<decltype(++it2)> itPlType;
  // TD<decltype(++it3)> it3Type;
}

int main() {
  TypeInfoCheck();
  return 0;
}