project(tasks C CXX)

include_directories(../../include/data_structures)
include_directories(../../include/algorithm)
include_directories(../../include/io)

add_library(data_structures STATIC
            region_finder.cpp)
