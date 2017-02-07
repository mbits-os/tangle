# We need thread support
find_package(Threads REQUIRED)

add_subdirectory(mods/googletest gtest)

set(GOOGLETEST_LIBS
	"${GTEST_LIB}"
	"${GMOCK_LIB}"
)

set(GTEST_LIB "$<TARGET_FILE:gtest>")
set(GMOCK_LIB "$<TARGET_FILE:gmock>")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mods/googletest/googletest/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/mods/googletest/googlemock/include")

set(GOOGLETEST_LIBS
	"${GTEST_LIB}"
	"${GMOCK_LIB}"
)

set_target_properties(gmock PROPERTIES FOLDER Googletest)
set_target_properties(gmock_main PROPERTIES FOLDER Googletest)
set_target_properties(gtest PROPERTIES FOLDER Googletest)
set_target_properties(gtest_main PROPERTIES FOLDER Googletest)
