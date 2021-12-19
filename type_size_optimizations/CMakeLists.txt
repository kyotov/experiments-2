add_library(type_size_optimizations price_setter.cc)
target_link_libraries(type_size_optimizations
    PRIVATE glog::glog)

add_library(simple_call simple_call.cc)
    target_link_libraries(simple_call
        PRIVATE glog::glog)

add_library(complex_call complex_call.cc)
    target_link_libraries(complex_call
        PRIVATE glog::glog)


add_executable(type_size_optimizations_demo type_size_optimizations_demo.cc)
target_link_libraries(type_size_optimizations_demo
    PRIVATE type_size_optimizations
    PRIVATE simple_call
    PRIVATE complex_call
    PRIVATE glog::glog)
