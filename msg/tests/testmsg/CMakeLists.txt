find_package(Threads)

file(GLOB SRCS *.cc)
if (MSVC)
list(APPEND SRCS ${NATVIS})
endif()

set(LIBS $<TARGET_FILE:msg_s> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

add_executable(testmsg ../main.cpp ${SRCS})
set_target_properties(testmsg PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(testmsg msg_s gtest gmock)
target_link_libraries(testmsg ${LIBS})

enable_testing()
add_test(NAME hash.samples COMMAND testmsg --gtest_filter=hashing/hasher.*)
add_test(NAME hash.partial-update COMMAND testmsg --gtest_filter=partial/hasher_parts.*)
add_test(NAME parser.base COMMAND testmsg --gtest_filter=samples/base_parser.*)
add_test(NAME parser.http.req COMMAND testmsg --gtest_filter=requests/request_parser.*)
add_test(NAME parser.http.resp COMMAND testmsg --gtest_filter=responses/response_parser.*)
add_test(NAME parser.http.errors COMMAND testmsg --gtest_filter=first_line/*)
