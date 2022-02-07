add_library(expressions2 utils.cc operator_node.cc expression.cc)
target_link_libraries(expressions2
    PRIVATE cast_utils
    PRIVATE glog::glog)

add_executable(expressions_tests2 expressions_tests.cc)
target_link_libraries(expressions_tests2
    PRIVATE glog::glog
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main
    PRIVATE expressions2)
gtest_discover_tests(expressions_tests2)
