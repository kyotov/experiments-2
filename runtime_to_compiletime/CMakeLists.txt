add_executable(demo ct.cc)
target_link_libraries(demo
    PRIVATE expressions2
    PRIVATE glog::glog)
