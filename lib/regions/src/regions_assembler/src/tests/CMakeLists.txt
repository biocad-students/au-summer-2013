cmake_minimum_required(VERSION 2.6)

project(tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
enable_testing()

set(CMAKE_CURRENT_BINARY_DIR "../bin")

find_package (Threads)

include_directories(/opt/local/include)
include_directories(../../include/utils)
include_directories(../../include/model)

link_directories(/opt/local/lib)
link_directories(${CMAKE_SOURCE_DIR}/utils)
link_directories(${CMAKE_SOURCE_DIR}/model)
link_directories(${CMAKE_SOURCE_DIR}/misc)
link_directories(${CMAKE_SOURCE_DIR}/gtest-1.6.0)

add_executable(tests
  tests.cpp)
  
target_link_libraries(tests gtest gtest_main utils model misc)

add_test(NAME tests COMMAND tests)

install(TARGETS tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
