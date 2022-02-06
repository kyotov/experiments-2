#include <glog/logging.h>
#include <gtest/gtest.h>

#include <limits>

#include "cast_utils.h"

// NOTE: Clang-tidy checks disabled for this file due to use of ASSERT_DEATH
namespace code_experiments {

class UtilsTests : public ::testing::Test {};

using UtilsDeathTests = UtilsTests;

TEST_F(UtilsTests, UlongToInt) {
  uint64_t ul = 5;
  int i = CheckedNumericCast<uint64_t, int>(ul);
  EXPECT_EQ(i, ul);
}

void OverflowTest() {
  auto ul = 1 + static_cast<uint64_t>(std::numeric_limits<int>::max());
  CheckedNumericCast<uint64_t, int>(ul);
}

void UnderflowTest() {
  auto ul = static_cast<uint64_t>(std::numeric_limits<int>::min()) - 1;
  CheckedNumericCast<uint64_t, int>(ul);
}

TEST_F(UtilsDeathTests, UlongToIntOverflow) {
  ASSERT_DEATH({ OverflowTest(); }, "overflow");
  ASSERT_DEATH({ UnderflowTest(); }, "underflow");
}

}  // namespace code_experiments