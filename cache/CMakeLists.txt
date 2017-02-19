cmake_minimum_required(VERSION 3.4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cstring/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../uri/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cookie/include)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

set(SOURCES
	src/cache.cpp
	src/loader.cpp

	include/tangle/cache/cache.h
	include/tangle/cache/loader.h
	)

WRAP_SOURCELIST(${SOURCES})

add_library(cache STATIC ${SOURCES})
set_target_properties(cache PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)

install(TARGETS cache
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)

##################################################################
##  DOCUMENTATION
##################################################################

if (DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(cache-doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating cache API documentation with Doxygen" VERBATIM)
	add_dependencies(doc cache-doc)
	set_target_properties(cache-doc PROPERTIES FOLDER Doxygen)
endif()

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)
if (MSVC)
list(APPEND TEST_SRCS ${NATVIS})
endif()

add_executable(cache-test ../googletest.cpp ${TEST_SRCS})
set_target_properties(cache-test PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(cache-test cache gtest gmock)
target_link_libraries(cache-test $<TARGET_FILE:cache> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()

endif()
