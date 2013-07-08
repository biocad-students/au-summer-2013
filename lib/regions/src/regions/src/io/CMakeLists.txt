project(io C CXX)

include_directories(../../include/io)
include_directories(../../include/data_structures)
include_directories(../../include/misc)

add_library(io STATIC
            fasta_reader.cpp)
