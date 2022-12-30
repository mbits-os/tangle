cmake_minimum_required(VERSION 3.4)
project(str VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
    src/str.cpp

    include/tangle/str.hpp
	include/tangle/base_parser.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_tangle_library(str ${SOURCES})

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(str-test ${TEST_SRCS})
target_compile_options(str-test PRIVATE ${ADDITIONAL_WALL_FLAGS})
set_target_properties(str-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(str-test str GTest::gtest_main)

add_test(NAME str COMMAND str-test)

endif()
