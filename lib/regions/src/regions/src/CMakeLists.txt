cmake_minimum_required(VERSION 2.6)

project(regions)
#find_package(TBB REQUIRED)
#find_package(LOG4CXX REQUIRED)
find_package(gtest REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

set(CMAKE_CURRENT_BINARY_DIR "../bin")
set(CMAKE_INSTALL_PREFIX "./")

add_subdirectory(algorithm)
add_subdirectory(data_structures)
add_subdirectory(ssw)
add_subdirectory(tests)
add_subdirectory(misc)
add_subdirectory(io)
add_subdirectory(utils)
add_subdirectory(tasks)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(/opt/local/include)
include_directories(../include)
include_directories(../include/ssw)
include_directories(../include/io)
include_directories(../include/misc)
include_directories(../include/utils)
include_directories(../include/algorithm)
include_directories(../include/data_structures)
include_directories(../include/tasks)

link_directories(${CMAKE_SOURCE_DIR}/ssw)
link_directories(${CMAKE_SOURCE_DIR}/io)
link_directories(${CMAKE_SOURCE_DIR}/misc)
link_directories(${CMAKE_SOURCE_DIR}/utils)
link_directories(${CMAKE_SOURCE_DIR}/algorithm)
link_directories(${CMAKE_SOURCE_DIR}/tasks)
link_directories(${Boost_LIBRARY_DIRS})

add_executable(regions
	main.cpp)

target_link_libraries(regions ssw algorithm misc tasks io utils data_structures z log4cxx ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

install(TARGETS regions DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(DIRECTORY "./config"
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        FILES_MATCHING 
        PATTERN "*.ini"
        PATTERN "*.properties")
install(DIRECTORY "./test_data"
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        FILES_MATCHING 
        PATTERN "*.fasta")
        