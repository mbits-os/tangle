cmake_minimum_required(VERSION 3.4)
project(msg VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/field_parser.cpp
	src/http_parser.cpp
	src/hasher.cpp

	include/tangle/msg/field_parser.hpp
	include/tangle/msg/http_parser.hpp
	include/tangle/msg/hasher.hpp
	)

WRAP_SOURCELIST(${SOURCES})

add_library(msg STATIC ${SOURCES})
set_target_properties(msg PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	FOLDER tangle)
target_include_directories(msg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(msg PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (MSVC)
	target_sources(msg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../natvis/msg.natvis)
endif()

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

add_executable(msg-test ${TEST_SRCS})
set_target_properties(msg-test PROPERTIES FOLDER tangle/tests)
target_link_libraries(msg-test msg GTest::gtest_main)

add_test(NAME msg COMMAND msg-test)

endif()
