cmake_minimum_required(VERSION 2.6)

project(tests)
find_package(gtest REQUIRED)
enable_testing()

set(CMAKE_CURRENT_BINARY_DIR "../bin")

find_package (Threads)

include_directories(/opt/local/include)
link_directories(/opt/local/lib)

add_executable(tests
  tests.cpp)
  
target_link_libraries(tests gtest gtest_main)
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME tests COMMAND tests)

install(TARGETS tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
