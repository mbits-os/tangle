# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and build GoogleTest
ExternalProject_Add(
    googletest
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
    GIT_REPOSITORY https://github.com/google/googletest
	# GIT_TAG release-1.7.0
    # Disable install step
    INSTALL_COMMAND ""
)

add_library(gtest IMPORTED STATIC GLOBAL)
add_library(gmock IMPORTED STATIC GLOBAL)
add_dependencies(gtest googletest)
add_dependencies(gmock googletest)

if (MSVC)
	macro(SET_LIBNAME var dir proj)
		set(${var} "${dir}/${CMAKE_CFG_INTDIR}/${proj}.lib")
	endmacro()
else()
	macro(SET_LIBNAME var dir proj)
		set(${var} "${dir}/lib${proj}.a")
	endmacro()
endif()

ExternalProject_Get_Property(googletest source_dir binary_dir)

SET_LIBNAME(GTEST_LIB "${binary_dir}/googlemock/gtest" gtest)
SET_LIBNAME(GMOCK_LIB "${binary_dir}/googlemock" gmock)

set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION "${GTEST_LIB}"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
)
set_target_properties(gmock PROPERTIES
    IMPORTED_LOCATION "${GMOCK_LIB}"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/googletest/include")
include_directories("${source_dir}/googlemock/include")

set(GOOGLETEST_LIBS
	"${GTEST_LIB}"
	"${GMOCK_LIB}"
)

add_subdirectory( testcookie )
