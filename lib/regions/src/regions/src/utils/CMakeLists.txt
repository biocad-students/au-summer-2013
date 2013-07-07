project(utils C CXX)

include_directories(../../include/utils)
include_directories(../../include/misc)

add_library(utils STATIC
            amino_acid_converter.cpp
            reverse_complement.cpp)

