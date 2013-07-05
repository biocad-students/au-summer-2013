project(algorithm C CXX)

include_directories(../../include)

add_library(algorithm STATIC
            aho_corasick.cpp)
