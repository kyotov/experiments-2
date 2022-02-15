#include <glog/logging.h>
#include <gtest/gtest.h>

#include <map>

#include "consts.h"
#include "zexpressionsv.h"

namespace code_experiments {

class ExpressionsTests : public ::testing::Test {};

TEST_F(ExpressionsTests, BasicOperatorCheck) {  // NOLINT
  std::string str{"C 3"};
  std::stringstream stream{str};
  auto e = CreateFromStream(stream);
  auto result = ExprEval(e);
  EXPECT_EQ(result, 3);
}

TEST_F(ExpressionsTests, BasicOperatorCheck2) {  // NOLINT
  std::string str{"BOp + C 1 C 3"};
  std::stringstream stream{str};
  auto e = CreateFromStream(stream);
  auto result = ExprEval(e);
  EXPECT_EQ(result, 4);
}

TEST_F(ExpressionsTests, BasicOperatorCheck3) {  // NOLINT
  std::string str{"BOp + BOp + C 1 C 3 C 6"};
  std::stringstream stream{str};
  auto e = CreateFromStream(stream);
  auto result = ExprEval(e);
  EXPECT_EQ(result, 10);
}

}  // namespace code_experiments