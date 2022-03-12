add_library(ca_expressions
    expressions.cc
)
set_target_properties(ca_expressions PROPERTIES CXX_CLANG_TIDY "")
#target_compile_options(ca_expressions PRIVATE -g3 -ggdb3 -Wall -Wpedantic -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wnoexcept -Wredundant-decls -Wshadow -Wsign-promo -Werror)
