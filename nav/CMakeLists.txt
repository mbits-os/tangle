cmake_minimum_required(VERSION 3.4)
project(nav VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/doc_impl.cpp
	src/document.cpp
	src/headers_parsers.cpp
	src/headers_parsers.hpp
	src/headers.cpp
	src/nav_jar.cpp
	src/navigator.cpp
	src/request.cpp

	include/tangle/nav/doc_impl.hpp
	include/tangle/nav/document.hpp
	include/tangle/nav/headers.hpp
	include/tangle/nav/jar.hpp
	include/tangle/nav/navigator.hpp
	include/tangle/nav/protocol.hpp
	include/tangle/nav/request.hpp
	)

WRAP_SOURCELIST(${SOURCES})

add_tangle_library(nav ${SOURCES})
target_link_libraries(nav PUBLIC uri cookie msg str)

if (MSVC)
	target_sources(nav PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../natvis/header.natvis)
endif()

##################################################################
##  DOCUMENTATION
##################################################################

if (DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(nav-doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating nav API documentation with Doxygen" VERBATIM)
	add_dependencies(doc nav-doc)
	set_target_properties(nav-doc PROPERTIES FOLDER Doxygen)
endif()

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(nav-test ${TEST_SRCS})
set_target_properties(nav-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(nav-test nav GTest::gmock_main)

add_test(NAME nav COMMAND nav-test)

endif()
