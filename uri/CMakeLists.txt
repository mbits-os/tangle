cmake_minimum_required(VERSION 3.4)
project(uri VERSION 1.0.0 LANGUAGES CXX )

set(SOURCES src/uri.cpp include/tangle/uri.hpp)

WRAP_SOURCELIST(${SOURCES})

add_library(uri STATIC ${SOURCES})
set_target_properties(uri PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)
target_include_directories(uri PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(uri PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (MSVC)
	target_sources(uri PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../natvis/uri.natvis)
endif()

if (TANGLE_INSTALL)
install(TARGETS uri
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
endif()

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
set_target_properties(uri-test PROPERTIES FOLDER tests)
target_link_libraries(uri-test uri GTest::gtest_main)

add_test(NAME href.empties COMMAND uri-test --gtest_filter=Empties/*)
add_test(NAME href.roots COMMAND uri-test --gtest_filter=Roots/*)
add_test(NAME href.here COMMAND uri-test --gtest_filter=Here/*)
add_test(NAME href.up COMMAND uri-test --gtest_filter=Up/*)
add_test(NAME href.opaque COMMAND uri-test --gtest_filter=Opaque/*)
add_test(NAME uri.authority COMMAND uri-test --gtest_filter=Authority/*)
add_test(NAME uri.path COMMAND uri-test --gtest_filter=Path/*)
add_test(NAME uri.relpath COMMAND uri-test --gtest_filter=RelPath/*)
add_test(NAME uri.query COMMAND uri-test --gtest_filter=Query/*)
add_test(NAME uri.makeBase COMMAND uri-test --gtest_filter=MakeBase/*)
add_test(NAME uri.parts COMMAND uri-test --gtest_filter=samples/uri_parts.*)
add_test(NAME uri.auth-parts COMMAND uri-test --gtest_filter=samples/uri_auth.*)
add_test(NAME uri.queries COMMAND uri-test --gtest_filter=samples/query.*)

endif()
