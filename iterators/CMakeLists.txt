add_library(primes primes.cc)

add_executable(iterator_demo iterator_demo.cc)
target_link_libraries(iterator_demo
    PRIVATE primes
    PRIVATE glog::glog)

add_executable(type_check_demo type_check_demo.cc)
target_link_libraries(type_check_demo
    PRIVATE primes
    PRIVATE glog::glog)

add_executable(primes_tests primes_tests.cc)
target_link_libraries(primes_tests
    PRIVATE primes
    PRIVATE glog::glog
    PRIVATE GTest::gtest
    PRIVATE GTest::gtest_main)
gtest_discover_tests(primes_tests)