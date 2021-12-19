#ifndef EXPERIMENTS_TYPE_SIZE_OPTIMIZATIONS_PRICE_SETTER_H
#define EXPERIMENTS_TYPE_SIZE_OPTIMIZATIONS_PRICE_SETTER_H

namespace code_experiments {

template <typename T>
class PriceSetter {
public:
  void SetPrices(T &t) const;
};

}  // namespace code_experiments

#endif