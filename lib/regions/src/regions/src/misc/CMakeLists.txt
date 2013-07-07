project(misc C CXX)

include_directories(../../include/misc)

add_library(misc STATIC
            config_reader.cpp
            generic_exception.cpp)

