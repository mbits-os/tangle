cmake_minimum_required(VERSION 3.4)
project(cache VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/cache.cpp
	src/loader.cpp

	include/tangle/cache/cache.h
	include/tangle/cache/loader.h
	)

WRAP_SOURCELIST(${SOURCES})

add_library(cache STATIC ${SOURCES})
set_target_properties(cache PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)
target_include_directories(cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(cache PUBLIC uri cookie)
target_compile_options(cache PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (TANGLE_INSTALL)
install(TARGETS cache
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

# add_executable(cache-test ${TEST_SRCS})
# set_target_properties(cache-test PROPERTIES FOLDER tests)
# target_link_libraries(cache-test cache GTest::gtest_main)

endif()
