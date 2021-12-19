#include "simple_call.h"

#include <glog/logging.h>

#include "prices.h"

namespace code_experiments {

void SimpleCall() {
  LOG(INFO) << "Making simple call";
  Prices prices{.bid = 0, .ask = 0};
  code_experiments::PriceSetter<Prices> price_setter;
  price_setter.SetPrices(prices);
}

}  // namespace code_experiments