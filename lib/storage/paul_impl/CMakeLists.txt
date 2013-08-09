cmake_minimum_required (VERSION 2.6)

project(IG_Container)

set (APP_NAME IG_Container)

set (HDR_DIR ${PROJECT_SOURCE_DIR}/contig)
	
set (TST_DIR ${PROJECT_SOURCE_DIR}/tests)

set (CMN_DIR ${PROJECT_SOURCE_DIR}/utils)
	
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set(CMAKE_BUILD_TYPE RELEASE)
IF(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
ENDIF(UNIX)
	
include_directories(
	${PROJECT_SOURCE_DIR}/utils
	${PROJECT_SOURCE_DIR}/contig
	${PROJECT_SOURCE_DIR}/../../share/zlib-1.2.8
)

FILE( GLOB SOURCES
	${CMN_DIR}/*.cpp
	*.cpp
)

FILE( GLOB COMMON
	${CMN_DIR}/*.h
	*.h
	)

FILE( GLOB ANNOTATION
	${HDR_DIR}/annotation/*.hpp
)

FILE( GLOB TRIE
	${HDR_DIR}/trie/*.hpp
)

FILE( GLOB KSTAT
	${HDR_DIR}/kstat/*.hpp
)

FILE( GLOB STORAGE
	${HDR_DIR}/*.hpp
)

FILE( GLOB ALIGN
	${HDR_DIR}/aligner/*.hpp
)

FILE( GLOB TESTS
	${TST_DIR}/*.cpp
	)

IF(UNIX)
SET ( REQUIRED_LIBRARIES
        z 
)
ELSEIF(WIN32)	
SET ( REQUIRED_LIBRARIES
	zlib
)
ENDIF(UNIX)

source_group ("Annotation Files" FILES ${ANNOTATION})
source_group ("Trie Files" FILES ${TRIE})
source_group ("Kstat Files" FILES ${KSTAT})
source_group ("Storage Files" FILES ${STORAGE})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Common Files" FILES ${COMMON})
source_group ("Test Files" FILES ${TESTS})
source_group ("Align Files" FILES ${ALIGN})

add_executable (${APP_NAME}
	${SOURCES}
	${ANNOTATION}
	${TRIE}
	${KSTAT}
	${STORAGE}
	${ALGORITHM}
	${COMMON}
	${TESTS}
	${ALIGN}
	)

target_link_libraries(${APP_NAME} 
	${REQUIRED_LIBRARIES}
	)
