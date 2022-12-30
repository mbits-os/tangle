cmake_minimum_required(VERSION 3.4)
project(browser VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
    src/entities.cpp
    src/fetch.cpp
    src/forms.cpp
    src/html_dtd.cpp
    src/html_split.cpp
    src/offline_media.cpp
    src/walk/action_parser.hpp
    src/walk/cursor.cpp
    src/walk/pages.cpp
    src/walk/selector_action.cpp
    src/walk/selector_parser.hpp
    src/walk/selector.cpp
    src/walk/std_actions.cpp

    include/tangle/browser/entities.hpp
    include/tangle/browser/fetch.hpp
    include/tangle/browser/forms.hpp
    include/tangle/browser/html_dtd_flags.hpp
    include/tangle/browser/html_dtd.hpp
    include/tangle/browser/html_split.hpp
    include/tangle/browser/offline_media.hpp
    include/tangle/browser/walk/cursor.hpp
    include/tangle/browser/walk/pages.hpp
    include/tangle/browser/walk/selector_action.hpp
    include/tangle/browser/walk/selector.hpp
    include/tangle/browser/walk/span.hpp
    include/tangle/browser/walk/std_actions.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_tangle_library(browser ${SOURCES})
target_link_libraries(browser str nav)

##################################################################
##  TESTING
##################################################################

if (TANGLE_TESTING)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests)

file(GLOB TEST_SRCS tests/*.cc)
file(GLOB WALK_TEST_SRCS tests/walk/*.cc)

add_executable(browser-test ${TEST_SRCS})
target_compile_options(browser-test PRIVATE ${ADDITIONAL_WALL_FLAGS})
set_target_properties(browser-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(browser-test browser GTest::gtest_main)

add_executable(browser-walk-test ${WALK_TEST_SRCS})
target_compile_options(browser-walk-test PRIVATE ${ADDITIONAL_WALL_FLAGS})
set_target_properties(browser-walk-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(browser-walk-test browser GTest::gtest_main)

add_test(NAME browser COMMAND browser-test)
add_test(NAME browser-walk COMMAND browser-walk-test)

endif()
