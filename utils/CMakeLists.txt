cmake_minimum_required(VERSION 3.4)
project(utils VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
    src/html_entities.cpp
    src/html_forms.cpp
    src/html_split.cpp
    src/str.cpp

    include/tangle/html_entities.hpp
    include/tangle/html_forms.hpp
    include/tangle/html_split.hpp
    include/tangle/str.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_library(utils STATIC ${SOURCES})
set_target_properties(utils PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER tangle)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(utils PRIVATE ${ADDITIONAL_WALL_FLAGS})

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(utils-test ${TEST_SRCS})
target_compile_options(utils-test PRIVATE ${ADDITIONAL_WALL_FLAGS})
set_target_properties(utils-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(utils-test utils GTest::gtest_main)

add_test(NAME utils COMMAND utils-test)

endif()
