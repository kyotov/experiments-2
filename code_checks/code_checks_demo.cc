#include <glog/logging.h>

#include <future>
#include <memory>
#include <vector>

// Uncommenting the below will cause the following compile error:
//   Address of stack memory associated with local variable 's' returned to
//   caller [clang-analyzer-core.StackAddressEscape]
//   ... [clang-diagnostic-return-stack-address]
// std::string& IncorrectRefCheck() {
//   std::string s("Hello");
//   return s;
// }

void UseAfterMoveCheck() {
  std::string s("UseAfterMoveCheck");
  auto t = std::move(s);
  LOG(INFO) << t;
  // Uncommenting the below LOG line will cause the following errors during
  // compile:
  //   bugprone-use-after-move,hicpp-invalid-access-moved
  // LOG(INFO) << s;
}

void NarrowingConversionCheck() {
  int32_t x = 0;
  // Uncommenting the below line will cause the following error during compile:
  //   bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions
  // int16_t y = x;
  // Expanding conversions continue to be accepted.
  int64_t z = x;
  LOG(INFO) << z;
}

void LocalReferenceToOtherThread() {
  std::string s("LocalReferenceToOtherThreadCheck");
  // NOTE: The below data race is currently not captured by clang tidy checks.
  // The below compiles and at least in one of the runs, the async run printed
  // an empty string.
  auto fut = std::async(std::launch::async, [&s]() { LOG(INFO) << s; });
  auto t = std::move(s);
  LOG(INFO) << t;
}

void LocalReferenceMovedToOtherThread() {
  std::string s("LocalReferenceMovedToOtherThread");
  // NOTE: The below data race is currently not captured by clang tidy checks.
  // The below compiles and at least in one of the runs, the async run printed
  // an empty string.
  auto fut =
      std::async(std::launch::async, [s = std::move(s)]() { LOG(INFO) << s; });
  // Uncommenting the below line will cause the following error during compile:
  //  Moved-from object 's' of type 'std::basic_string' is copied
  //  clang-analyzer-cplusplus.Move
  // auto t = s;
}

void ModifyWhileIterating(const std::vector<int> &vec) {
  for (auto val : vec) {
    LOG(INFO) << "Vector has value: " << val;
    // Unsurprisingly, uncommenting the below does raise compile errors:
    // vec.push_back(0);
  }
}

void ModifyWhileIterating() {
  std::vector<int> vec(2, 1);
  // NOTE: No clang checks seem to flag modify while iterating behavior
  for (auto val : vec) {
    LOG(INFO) << "Vector has value: " << val;
    vec.push_back(0);
  }
  LOG(INFO) << "After initial loop";
  for (auto val : vec) {
    LOG(INFO) << "Vector has value: " << val;
  }
  ModifyWhileIterating(vec);
}

void StringViewOnTempObject() {
  // Uncommenting the below three lines produces the following error:
  //   error: object backing the pointer will be destroyed at the end of the
  //   full-expression [clang-diagnostic-dangling-gsl,-warnings-as-errors]
  // std::string s = "Hellooooooo";
  // std::string_view sv = s + " World!";
  // LOG(INFO) << sv;

  // Uncommenting the below three lines produces the following error:
  //   error: object backing the pointer will be destroyed at the end of the
  //   full-expression [clang-diagnostic-dangling-gsl,-warnings-as-errors]
  // auto flag = false;
  // auto const name = flag ? std::string_view("Hello") : std::string("World");
  // LOG(INFO) << "Name is " << name;
}

int main() {
  LOG(INFO) << "Start of code checks demo.";
  // IncorrectRefCheck();
  UseAfterMoveCheck();
  NarrowingConversionCheck();
  LocalReferenceToOtherThread();
  LocalReferenceMovedToOtherThread();
  ModifyWhileIterating();
  StringViewOnTempObject();
  LOG(INFO) << "Done.";
  return 0;
}