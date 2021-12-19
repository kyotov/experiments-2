#include "complex_call.h"

#include <glog/logging.h>

#include "prices_with_band.h"

namespace code_experiments {

void ComplexCall() {
  LOG(INFO) << "Making complex call";
  PricesWithBand prices{.bid = 0, .ask = 0, .luld_lower = 0, .luld_upper = 0};
  code_experiments::PriceSetter<PricesWithBand> price_setter;
  price_setter.SetPrices(prices);
}

} // namespace code_experiments