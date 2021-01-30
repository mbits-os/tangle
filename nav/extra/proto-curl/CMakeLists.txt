cmake_minimum_required(VERSION 3.10)
project(proto-curl VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/curl.cpp
	include/tangle/curl/curl.hpp
	include/tangle/curl/proto.hpp
)

WRAP_SOURCELIST(${SOURCES})

add_library(proto-curl STATIC ${SOURCES})
set_target_properties(proto-curl PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    FOLDER libs/extras
)
target_include_directories(proto-curl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(proto-curl PUBLIC nav CURL::libcurl)
target_compile_options(proto-curl PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (TANGLE_INSTALL)
install(TARGETS proto-curl
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
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
set_target_properties(proto-curl-test tiny-process-library PROPERTIES FOLDER tests/extras)
target_link_libraries(proto-curl-test proto-curl GTest::gmock_main tiny-process-library)

add_test(NAME curl COMMAND proto-curl-test)

endif()
