cmake_minimum_required(VERSION 3.4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

enable_testing()
add_subdirectory( tests/testcookie )

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

set(SOURCES
	src/cookie_item.cpp
	src/cookie_scope.cpp
	src/cookie_jar.cpp
	src/cookie_chrono.cpp

	include/tangle/cookie.h
	include/tangle/cookie/item.h
	include/tangle/cookie/scope.h
	include/tangle/cookie/cstring.h
	include/tangle/cookie/jar.h
	include/tangle/cookie/io.h
	include/tangle/cookie/chrono.h
	)

WRAP_SOURCELIST(${SOURCES})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(cookie SHARED ${SOURCES})
add_library(cookie_s STATIC ${SOURCES})
set_target_properties(cookie PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/dynamic)
set_target_properties(cookie_s PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/static)

install(TARGETS cookie cookie_s
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
