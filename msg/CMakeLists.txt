cmake_minimum_required(VERSION 3.4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cstring/include)

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
	src/base_parser.cpp
	src/http_parser.cpp
	src/hasher.cpp

	include/tangle/msg/base_parser.h
	include/tangle/msg/http_parser.h
	include/tangle/msg/hasher.h
	)

WRAP_SOURCELIST(${SOURCES})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(msg SHARED ${SOURCES})
add_library(msg_s STATIC ${SOURCES})
set_target_properties(msg PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/dynamic)
set_target_properties(msg_s PROPERTIES
	CXX_STANDARD 14
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER libs/static)

install(TARGETS msg msg_s
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)

##################################################################
##  TESTING
##################################################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)
if (MSVC)
list(APPEND TEST_SRCS ${NATVIS})
endif()

add_executable(msg-test ../googletest.cpp ${TEST_SRCS})
set_target_properties(msg-test PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(msg-test msg_s gtest gmock)
target_link_libraries(msg-test $<TARGET_FILE:msg_s> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(NAME hash.samples COMMAND msg-test --gtest_filter=hashing/hasher.*)
add_test(NAME hash.partial-update COMMAND msg-test --gtest_filter=partial/hasher_parts.*)
add_test(NAME parser.base COMMAND msg-test --gtest_filter=samples/base_parser.*)
add_test(NAME parser.http.req COMMAND msg-test --gtest_filter=requests/request_parser.*)
add_test(NAME parser.http.resp COMMAND msg-test --gtest_filter=responses/response_parser.*)
add_test(NAME parser.http.errors COMMAND msg-test --gtest_filter=first_line/*)
