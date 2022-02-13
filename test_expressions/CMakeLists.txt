add_executable(test_expressions
    test_expressions.cc
)
target_link_libraries(test_expressions
    glog::glog
    GTest::gtest
    GTest::gtest_main
    ky_expressions
    expressions2
)
gtest_discover_tests(test_expressions)
set_target_properties(test_expressions PROPERTIES CXX_CLANG_TIDY "")