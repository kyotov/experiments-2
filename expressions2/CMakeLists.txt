add_library(expressions2 operator_node.cc expression.cc)
target_link_libraries(expressions2
    PRIVATE glog::glog)
