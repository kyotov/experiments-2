#include <glog/logging.h>

#include "complex_call.h"
#include "price_setter.h"
#include "prices.h"
#include "prices_with_band.h"
#include "simple_call.h"

template <typename T>
void SetPrices(T &t) {
  code_experiments::PriceSetter<T> price_setter;
  price_setter.SetPrices(t);
}

void LocalDemoCalls() {
  code_experiments::Prices prices{.bid = 0, .ask = 0};
  SetPrices(prices);

  code_experiments::PricesWithBand prices_with_band{
      .bid = 0,
      .ask = 0,
      .luld_lower = 0,
      .luld_upper = 0};
  SetPrices(prices_with_band);
}

int main() {
  LOG(INFO) << "Type size optimizations demo.";
  LocalDemoCalls();

  code_experiments::SimpleCall();
  code_experiments::ComplexCall();
  return 0;
}