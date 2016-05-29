cmake_minimum_required(VERSION 3.4)
project(libs-cookie CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

enable_testing()
add_subdirectory( tests )

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

add_definitions("-DGSL_THROW_ON_CONTRACT_VIOLATION")

set(SOURCES
	src/cookie_item.cpp
	src/cookie_scope.cpp
	src/cookie_jar.cpp
	src/cookie_chrono.cpp

	include/net/cookie.h
	include/net/cookie/item.h
	include/net/cookie/scope.h
	include/net/cookie/cstring.h
	include/net/cookie/jar.h
	include/net/cookie/io.h
	include/net/cookie/chrono.h
	)

macro(WRAP_SOURCELIST)
	foreach (_file ${ARGN})
		get_filename_component(_basename ${_file} NAME_WE)
		get_filename_component(_path ${_file} PATH)

		string(REGEX REPLACE "/" "\\\\\\\\" _sourcegroup "${_path}")
		source_group("${_sourcegroup}" FILES ${_file})
	endforeach ()
endmacro()

WRAP_SOURCELIST(${SOURCES})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(cookie SHARED ${SOURCES})
add_library(cookie_s STATIC ${SOURCES})
set_target_properties(cookie PROPERTIES
        CXX_STANDARD 14
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(cookie_s PROPERTIES
        CXX_STANDARD 14
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS cookie cookie_s
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/net/ DESTINATION include/net)
