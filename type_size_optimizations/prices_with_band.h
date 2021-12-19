#ifndef EXPERIMENTS_TYPE_SIZE_OPTIMIZATIONS_PRICES_WITH_BAND_H_

namespace code_experiments {

struct PricesWithBand {
  double bid;
  double ask;
  double luld_lower;
  double luld_upper;
};

}  // namespace code_experiments

#define EXPERIMENTS_TYPE_SIZE_OPTIMIZATIONS_PRICES_WITH_BAND_H_
#endif