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

set(SOURCES
	src/cookie_item.cpp
	src/cookie_scope.cpp
	src/cookie_jar.cpp
	src/cookie_chrono.cpp

	include/tangle/cookie.h
	include/tangle/cookie/item.h
	include/tangle/cookie/scope.h
	include/tangle/cookie/jar.h
	include/tangle/cookie/io.h
	include/tangle/cookie/chrono.h
	)

WRAP_SOURCELIST(${SOURCES})

add_library(cookie STATIC ${SOURCES})
set_target_properties(cookie PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)

install(TARGETS cookie
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)
list(APPEND TEST_SRCS tests/testing_io.h)
if (MSVC)
list(APPEND TEST_SRCS ${NATVIS})
endif()

add_executable(cookie-test ../googletest.cpp ${TEST_SRCS})
set_target_properties(cookie-test PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(cookie-test cookie gtest gmock)
target_link_libraries(cookie-test $<TARGET_FILE:cookie> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(NAME domains.scope COMMAND cookie-test --gtest_filter=domains/scope.*)
add_test(NAME paths.scope COMMAND cookie-test --gtest_filter=paths/scope.*)
add_test(NAME client.cookies COMMAND cookie-test --gtest_filter=client/cookies.*)
add_test(NAME flags.cookies COMMAND cookie-test --gtest_filter=flags/server_cookies.*)
add_test(NAME scope.cookies COMMAND cookie-test --gtest_filter=scope/server_cookies.*)
add_test(NAME expires.cookies COMMAND cookie-test --gtest_filter=expires/server_cookies.*)
add_test(NAME max_age.cookies COMMAND cookie-test --gtest_filter=max_age/server_cookies.*)
add_test(NAME client.header COMMAND cookie-test --gtest_filter=client/cookie_header.*)
add_test(NAME flags.header COMMAND cookie-test --gtest_filter=flags/SetCookie_header.*)
add_test(NAME scope.header COMMAND cookie-test --gtest_filter=scope/SetCookie_header.*)
add_test(NAME expires.header COMMAND cookie-test --gtest_filter=expires/SetCookie_header.*)
add_test(NAME max_age.header COMMAND cookie-test --gtest_filter=max_age/SetCookie_header.*)
add_test(NAME cookie.jar COMMAND cookie-test --gtest_filter=*/cookie_jar_*)

endif()