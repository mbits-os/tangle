cmake_minimum_required(VERSION 3.4)
project(uri VERSION 1.0.0 LANGUAGES CXX )

set(SOURCES
	src/uri.cpp
	src/decode.cpp

	include/tangle/uri.hpp
	include/tangle/decode.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_tangle_library(uri ${SOURCES})
add_natvis(uri uri)

##################################################################
##  DOCUMENTATION
##################################################################

if (DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../Doxyfile.in
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

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(uri-test ${TEST_SRCS})
set_target_properties(uri-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(uri-test uri_target GTest::gtest_main)

add_test(NAME uri COMMAND uri-test)

endif()
