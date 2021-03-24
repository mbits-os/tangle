cmake_minimum_required(VERSION 3.4)
project(browser VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
    src/action_parser.hpp
    src/cursor.cpp
    src/entities.cpp
    src/fetch.cpp
    src/forms.cpp
    src/html_split.cpp
    src/offline_media.cpp
    src/selector_action.cpp
    src/selector_parser.hpp
    src/selector.cpp
    src/html_dtd.cpp

    include/tangle/browser/cursor.hpp
    include/tangle/browser/entities.hpp
    include/tangle/browser/fetch.hpp
    include/tangle/browser/forms.hpp
    include/tangle/browser/html_split.hpp
    include/tangle/browser/offline_media.hpp
    include/tangle/browser/selector_action.hpp
    include/tangle/browser/selector.hpp
    include/tangle/browser/html_dtd.hpp
    include/tangle/browser/html_dtd_flags.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_library(browser STATIC ${SOURCES})
set_target_properties(browser PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER tangle)
target_include_directories(browser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(browser PRIVATE ${ADDITIONAL_WALL_FLAGS})
target_link_libraries(browser str nav)

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)

add_executable(browser-test ${TEST_SRCS})
target_compile_options(browser-test PRIVATE ${ADDITIONAL_WALL_FLAGS})
set_target_properties(browser-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(browser-test browser GTest::gtest_main)

add_test(NAME browser COMMAND browser-test)

endif()
