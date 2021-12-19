add_executable(multi_reader_async_read_only_demo multi_reader_async_read_only.cc)
target_link_libraries(multi_reader_async_read_only_demo
    PRIVATE glog::glog
)

add_executable(multi_reader_async_with_write_demo multi_reader_async_with_write.cc)
target_link_libraries(multi_reader_async_with_write_demo
    PRIVATE glog::glog
)