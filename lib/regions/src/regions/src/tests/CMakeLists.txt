cmake_minimum_required(VERSION 2.6)

project(tests)
find_package(gtest REQUIRED)
enable_testing()

set(CMAKE_CURRENT_BINARY_DIR "../bin")

find_package (Threads)

include_directories(/opt/local/include)
include_directories(../../include/utils)

link_directories(/opt/local/lib)
link_directories(${CMAKE_SOURCE_DIR}/utils)
link_directories(${CMAKE_SOURCE_DIR}/misc)

add_executable(tests
  tests.cpp)
  
target_link_libraries(tests log4cxx gtest gtest_main misc utils)
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME tests COMMAND tests)

install(TARGETS tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
