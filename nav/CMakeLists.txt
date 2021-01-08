cmake_minimum_required(VERSION 3.4)
project(nav VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/nav_jar.cpp
	src/navigator.cpp

	include/tangle/nav/jar.hpp
	include/tangle/nav/request.hpp
	include/tangle/nav/navigator.hpp
	include/tangle/nav/protocol.hpp
	)

WRAP_SOURCELIST(${SOURCES})

add_library(nav STATIC ${SOURCES})
set_target_properties(nav PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)
target_include_directories(nav PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nav PUBLIC uri cookie cache msg)
target_compile_options(nav PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (TANGLE_INSTALL)
install(TARGETS nav
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
if (MSVC)
list(APPEND TEST_SRCS ${NATVIS})
endif()

add_executable(nav-test ${TEST_SRCS})
set_target_properties(nav-test PROPERTIES FOLDER tests)
target_link_libraries(nav-test nav GTest::gmock_main)

endif()
