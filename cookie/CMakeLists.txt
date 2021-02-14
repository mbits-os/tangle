cmake_minimum_required(VERSION 3.4)
project(cookie VERSION 1.0.0 LANGUAGES CXX )

set(SOURCES
	src/cookie_chrono.cpp
	src/cookie_io.cpp
	src/cookie_item.cpp
	src/cookie_jar.cpp
	src/cookie_scope.cpp

	include/tangle/cookie.hpp
	include/tangle/cookie/chrono.hpp
	include/tangle/cookie/io.hpp
	include/tangle/cookie/item.hpp
	include/tangle/cookie/jar.hpp
	include/tangle/cookie/scope.hpp
	)

WRAP_SOURCELIST(${SOURCES})

add_library(cookie STATIC ${SOURCES})
set_target_properties(cookie PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER tangle)
target_include_directories(cookie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(cookie PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (TANGLE_INSTALL)
install(TARGETS cookie
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
	add_custom_target(cookie-doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating cookie API documentation with Doxygen" VERBATIM)
	add_dependencies(doc cookie-doc)
	set_target_properties(cookie-doc PROPERTIES FOLDER Doxygen)
endif()

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/cookie-dir.in.hh ${CMAKE_CURRENT_BINARY_DIR}/tests/cookie-dir.hh @ONLY)

file(GLOB TEST_SRCS tests/*.cc)
list(APPEND TEST_SRCS tests/testing_io.hpp)

add_executable(cookie-test ${TEST_SRCS})
set_target_properties(cookie-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(cookie-test cookie GTest::gtest_main)

add_test(NAME cookies COMMAND cookie-test)

endif()
