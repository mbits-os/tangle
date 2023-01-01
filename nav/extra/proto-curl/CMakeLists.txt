cmake_minimum_required(VERSION 3.10)
project(proto-curl VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/curl.cpp
	include/tangle/curl/curl.hpp
	include/tangle/curl/proto.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_tangle_library(proto-curl ${SOURCES})
tangle_link_libraries(proto-curl nav)
if (TARGET tangle)
	target_link_libraries(tangle PUBLIC CURL::libcurl)
else()
	target_link_libraries(proto-curl PUBLIC CURL::libcurl)
endif()

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)
add_subdirectory(tests/tiny-process-library)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/flask-app-dir.in.hh ${CMAKE_CURRENT_BINARY_DIR}/tests/flask-app-dir.hh @ONLY)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(proto-curl-test ${TEST_SRCS})
set_target_properties(proto-curl-test tiny-process-library PROPERTIES FOLDER tangle/extras/tests)
target_link_libraries(proto-curl-test proto-curl_target GTest::gmock_main tiny-process-library)

# add_test(NAME curl COMMAND proto-curl-test)

endif()
