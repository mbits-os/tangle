cmake_minimum_required(VERSION 3.4)

set(DEPS uri cookie msg)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cstring/include)

foreach(DEP ${DEPS})
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../${DEP}/include")
endforeach()

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
	src/nav_cache.cpp
	src/nav_jar.cpp
	src/navigator.cpp
	src/loader.cpp

	include/tangle/nav/cache.h
	include/tangle/nav/jar.h
	include/tangle/nav/loader.h
	include/tangle/nav/request.h
	include/tangle/nav/navigator.h
	include/tangle/nav/protocol.h
	)

WRAP_SOURCELIST(${SOURCES})

add_library(nav STATIC ${SOURCES})
set_target_properties(nav PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)

foreach(DEP ${DEPS})
	add_dependencies(nav ${DEP})
	target_link_libraries(nav $<TARGET_FILE:${DEP}>)
endforeach()

install(TARGETS nav
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

add_executable(nav-test ../googletest.cpp ${TEST_SRCS})
set_target_properties(nav-test PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(nav-test nav gtest gmock)
target_link_libraries(nav-test $<TARGET_FILE:nav> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})
foreach(DEP ${DEPS})
	add_dependencies(nav-test ${DEP})
	target_link_libraries(nav-test $<TARGET_FILE:${DEP}>)
endforeach()

enable_testing()
endif()
