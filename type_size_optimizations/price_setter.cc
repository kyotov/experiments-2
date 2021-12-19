#include "price_setter.h"

#include <glog/logging.h>

#include "prices.h"
#include "prices_with_band.h"

namespace code_experiments {

template <typename T>
void PriceSetter<T>::SetPrices(T &t) const {
  LOG(INFO) << "Setting prices";
  t.bid = 0;
  t.ask = 0;
  LOG(INFO) << "Set prices for struct with size: " << sizeof(T);
}

template class PriceSetter<Prices>;
template class PriceSetter<PricesWithBand>;

}  // namespace code_experiments