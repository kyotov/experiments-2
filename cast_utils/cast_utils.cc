#include "cast_utils.h"

#include <glog/logging.h>

#include <typeinfo>

namespace code_experiments {

template <typename From, typename To>
To CheckedNumericCast(From from) {
  auto to = static_cast<To>(from);
  CHECK(to == from) << "Encountered overflow or underflow when converting "
                    << from << " of type " << typeid(from).name() << " to "
                    << typeid(to).name();
  return to;
}

template int CheckedNumericCast<uint64_t, int>(uint64_t);

};  // namespace code_experiments