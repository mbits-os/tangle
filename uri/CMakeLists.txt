cmake_minimum_required(VERSION 3.4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cstring/include)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

set(SOURCES src/uri.cpp include/tangle/uri.h)

WRAP_SOURCELIST(${SOURCES})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(url SHARED ${SOURCES})
add_library(url_s STATIC ${SOURCES})
set_target_properties(url PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/dynamic)
set_target_properties(url_s PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/static)

install(TARGETS url url_s
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
	add_custom_target(uri-doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating uri API documentation with Doxygen" VERBATIM)
	add_dependencies(doc uri-doc)
	set_target_properties(uri-doc PROPERTIES FOLDER Doxygen)
endif()

##################################################################
##  TESTING
##################################################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)
if (MSVC)
list(APPEND TEST_SRCS ${NATVIS})
endif()

add_executable(url-test ../googletest.cpp ${TEST_SRCS})
set_target_properties(url-test PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(url-test url_s gtest gmock)
target_link_libraries(url-test $<TARGET_FILE:url_s> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(NAME href.empties COMMAND url-test --gtest_filter=Empties/*)
add_test(NAME href.roots COMMAND url-test --gtest_filter=Roots/*)
add_test(NAME href.here COMMAND url-test --gtest_filter=Here/*)
add_test(NAME href.up COMMAND url-test --gtest_filter=Up/*)
add_test(NAME uri.authority COMMAND url-test --gtest_filter=Authority/*)
add_test(NAME uri.path COMMAND url-test --gtest_filter=Path/*)
add_test(NAME uri.relpath COMMAND url-test --gtest_filter=RelPath/*)
add_test(NAME uri.query COMMAND url-test --gtest_filter=Query/*)
add_test(NAME uri.makeBase COMMAND url-test --gtest_filter=MakeBase/*)
