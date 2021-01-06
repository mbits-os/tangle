cmake_minimum_required(VERSION 3.4)
project(msg VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/base_parser.cpp
	src/http_parser.cpp
	src/hasher.cpp

	include/tangle/msg/base_parser.h
	include/tangle/msg/http_parser.h
	include/tangle/msg/hasher.h
	)

WRAP_SOURCELIST(${SOURCES})

if (MSVC)
	list(APPEND SOURCES ../natvis/msg.natvis)
endif()

add_library(msg STATIC ${SOURCES})
set_target_properties(msg PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs)
target_include_directories(msg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (TANGLE_INSTALL)
install(TARGETS msg
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
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

add_executable(msg-test ${TEST_SRCS})
set_target_properties(msg-test PROPERTIES FOLDER tests)
target_link_libraries(msg-test msg GTest::gtest_main)

add_test(NAME hash.samples COMMAND msg-test --gtest_filter=hashing/hasher.*)
add_test(NAME hash.partial-update COMMAND msg-test --gtest_filter=partial/hasher_parts.*)
add_test(NAME parser.base COMMAND msg-test --gtest_filter=samples/base_parser.*)
add_test(NAME parser.http.req COMMAND msg-test --gtest_filter=requests/request_parser.*)
add_test(NAME parser.http.resp COMMAND msg-test --gtest_filter=responses/response_parser.*)
add_test(NAME parser.http.errors COMMAND msg-test --gtest_filter=first_line/*)

endif()
