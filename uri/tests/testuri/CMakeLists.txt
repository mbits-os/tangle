find_package(Threads)

file(GLOB SRCS *.cc)

set(LIBS $<TARGET_FILE:url_s> ${GOOGLETEST_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
endif()

add_executable(testurl ../main.cpp ${SRCS})
set_target_properties(testurl PROPERTIES CXX_STANDARD 14 FOLDER tests)
add_dependencies(testurl url_s gtest gmock)
target_link_libraries(testurl ${LIBS})

enable_testing()
add_test(NAME Href.Empties COMMAND testurl --gtest_filter=Empties/*)
add_test(NAME Href.Roots COMMAND testurl --gtest_filter=Roots/*)
add_test(NAME Href.Here COMMAND testurl --gtest_filter=Here/*)
add_test(NAME Href.Up COMMAND testurl --gtest_filter=Up/*)
add_test(NAME Uri.Authority COMMAND testurl --gtest_filter=Authority/*)
add_test(NAME Uri.Path COMMAND testurl --gtest_filter=Path/*)
add_test(NAME Uri.RelPath COMMAND testurl --gtest_filter=RelPath/*)
add_test(NAME Uri.Query COMMAND testurl --gtest_filter=Query/*)
add_test(NAME Uri.MakeBase COMMAND testurl --gtest_filter=MakeBase/*)
