project(data_structures C CXX)

include_directories(../../include/data_structures)

add_library(data_structures STATIC
            database.cpp)
