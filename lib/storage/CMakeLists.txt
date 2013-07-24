cmake_minimum_required (VERSION 2.6)

project(IG_Storage)

set (APP_NAME IG_Storage)
	
set (HDR_DIR ${PROJECT_SOURCE_DIR}/include)
	
set (TST_DIR ${PROJECT_SOURCE_DIR}/tests)

set (CMN_DIR ${PROJECT_SOURCE_DIR}/common)
	
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set(CMAKE_BUILD_TYPE RELEASE)

IF(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
ENDIF(UNIX)
	
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/common/include
	#{PROJECT_SOURCE_DIR}/tests/include
	${PROJECT_SOURCE_DIR}/../share/zlib-1.2.8
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

SET( COMMON_SOURCES
	${CMN_DIR}/sources/fasta_reader.cpp
)

SET ( COMMON_INCLUDE
	${CMN_DIR}/include/fasta_reader.h
	${CMN_DIR}/include/kseq.h
	${CMN_DIR}/include/read.h
	${CMN_DIR}/include/scorematrix.h
	)

SET ( TEST_SOURCES
	${TST_DIR}/sources/align_unittest.cpp
	${TST_DIR}/sources/annotation_unittest.cpp
	${TST_DIR}/sources/find_unitttest.cpp
	${TST_DIR}/sources/kstat_unittest.cpp
	${TST_DIR}/sources/matrix_unittest.cpp
	${TST_DIR}/sources/storage_unittest.cpp
	)
	
SET ( TEST_INCLUDE
	${TST_DIR}/include/unittest.h
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
source_group ("Common Files" FILES ${COMMON_SOURCES} ${COMMON_INCLUDE})
source_group ("Test Files" FILES ${TEST_SOURCES} ${TEST_INCLUDE})

add_executable (${APP_NAME}
	${TEST_SOURCES}
	${TEST_INCLUDE}
	${ANNOTATION}
	${TRIE}
	${KSTAT}
	${STORAGE}
	${COMMON_SOURCES}
	${COMMON_INCLUDE}
	main.cpp
	)

target_link_libraries(${APP_NAME} 
	${REQUIRED_LIBRARIES}
	)