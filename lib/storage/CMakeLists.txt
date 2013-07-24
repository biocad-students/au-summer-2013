cmake_minimum_required (VERSION 2.6)

project(IG_Storage)

set (APP_NAME IG_Storage)
	
set (SRC_DIR ${PROJECT_SOURCE_DIR}/sources)
	
set (HDR_DIR ${PROJECT_SOURCE_DIR}/include)
	
set (TST_DIR ${PROJECT_SOURCE_DIR}/tests)

set (CMN_DIR ${PROJECT_SOURCE_DIR}/common)
	
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set(CMAKE_BUILD_TYPE DEBUG)
IF(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
ENDIF(UNIX)
	
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/common
	${PROJECT_SOURCE_DIR}/../share/zlib-1.2.8
)

SET( SOURCES
	${SRC_DIR}/fasta_reader.cpp
	${SRC_DIR}/scorematrix.cpp
)

SET( ANNOTATION
	${HDR_DIR}/annotation/annotation.hpp
	${HDR_DIR}/annotation/annotation_node.hpp
	${HDR_DIR}/annotation/annotation_record.hpp
)

SET( TRIE
	${HDR_DIR}/trie/trie.hpp
	${HDR_DIR}/trie/trie_topology.hpp
)

SET( KSTAT
	${HDR_DIR}/kstat/kstat.hpp
)

SET( STORAGE
	${HDR_DIR}/storage/storage.hpp
)

SET( ALGORITHM
	${HDR_DIR}/algorithm/algorithm.hpp
)

SET ( COMMON
	${CMN_DIR}/fasta_reader.h
	${CMN_DIR}/kseq.h
	${CMN_DIR}/read.h
	${CMN_DIR}/scorematrix.h
	)

SET ( TESTS
	${TST_DIR}/unittest.cpp
	)
	
IF(WIN32)
SET ( REQUIRED_LIBRARIES
	zlib
)
ELSEIF(UNIX)
SET ( REQUIRED_LIBRARIES
	z
)
ENDIF(WIN32)

source_group ("Annotation Files" FILES ${ANNOTATION})
source_group ("Trie Files" FILES ${TRIE})
source_group ("Kstat Files" FILES ${KSTAT})
source_group ("Storage Files" FILES ${STORAGE})
source_group ("Algorithm Files" FILES ${ALGORITHM})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Common Files" FILES ${COMMON})
source_group ("Test Files" FILES ${TESTS})

add_executable (${APP_NAME}
	${SOURCES}
	${ANNOTATION}
	${TRIE}
	${KSTAT}
	${STORAGE}
	${COMMON}
	${TESTS}
	)

target_link_libraries(${APP_NAME} 
	${REQUIRED_LIBRARIES}
	)
