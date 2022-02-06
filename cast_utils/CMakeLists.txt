add_library(cast_utils cast_utils.cc)
target_link_libraries(cast_utils
    PRIVATE glog::glog)

add_executable(cast_utils_tests cast_utils_tests.cc)
target_link_libraries(cast_utils_tests
    PRIVATE cast_utils
    PRIVATE glog::glog
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main)
set_target_properties(cast_utils_tests PROPERTIES CXX_CLANG_TIDY "")
gtest_discover_tests(cast_utils_tests)
