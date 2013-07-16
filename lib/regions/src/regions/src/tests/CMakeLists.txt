cmake_minimum_required(VERSION 2.6)

project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ${OpenMP_CXX_FLAGS} -std=c++11")
find_package(gtest REQUIRED)
enable_testing()

set(CMAKE_CURRENT_BINARY_DIR "../bin")

find_package (Threads)

include_directories(/opt/local/include)
include_directories(../../include/utils)
include_directories(../../include/io)
include_directories(../../include/data_structures)
include_directories(../../include/ssw)
include_directories(../../include/misc)

link_directories(/opt/local/lib)
link_directories(${CMAKE_SOURCE_DIR}/data_structures)
link_directories(${CMAKE_SOURCE_DIR}/utils)
link_directories(${CMAKE_SOURCE_DIR}/misc)
link_directories(${CMAKE_SOURCE_DIR}/io)
link_directories(${CMAKE_SOURCE_DIR}/ssw)

add_executable(tests
  tests.cpp)
  
target_link_libraries(tests log4cxx gtest gtest_main misc utils io z ssw tbb data_structures)
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME tests COMMAND tests)

install(TARGETS tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
