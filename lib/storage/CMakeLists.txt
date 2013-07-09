cmake_minimum_required (VERSION 2.6)

project(
	IG_Storage
	)

set ( SRC_DIR
	${PROJECT_SOURCE_DIR}/sources
	)
	
set ( HDR_DIR
	${PROJECT_SOURCE_DIR}/include
	)
	
set ( TST_DIR
	${PROJECT_SOURCE_DIR}/tests
	)

set ( CMN_DIR
	${PROJECT_SOURCE_DIR}/common
	)
	
set ( APP_NAME 
	IG_Storage
	)

# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE DEBUG )
	
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/common
)

SET( SOURCES
	${SRC_DIR}/fasta_reader.cpp
)

SET( HEADERS
	${HDR_DIR}/trie.hpp
	${HDR_DIR}/contig.hpp
)

set ( COMMON
	${CMN_DIR}/fasta_reader.h
	${CMN_DIR}/kseq.h
	${CMN_DIR}/read.h
	)

SET ( TESTS
	${TST_DIR}/unittest.cpp
	)

SET ( REQUIRED_LIBRARIES
	zlib
)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Common Files" FILES ${COMMON})
source_group ("Test Files" FILES ${TESTS})

add_executable (${PROJECT_NAME}
	${SOURCES}
	${HEADERS}
	${COMMON}
	${TESTS}
	)
	
target_link_libraries(${REQUIRED_LIBRARIES})