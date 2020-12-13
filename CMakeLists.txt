cmake_minimum_required(VERSION 3.4)
project( tangle VERSION 0.1.0 LANGUAGES CXX )

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

if (NOT TARGET CURL::libcurl)
find_package(ZLIB REQUIRED CONFIG HINTS ${PROJECT_BINARY_DIR})
find_package(OpenSSL REQUIRED CONFIG HINTS ${PROJECT_BINARY_DIR})
find_package(CURL REQUIRED CONFIG HINTS ${PROJECT_BINARY_DIR})
endif()

set(TANGLE_WALL_FLAGS ON CACHE BOOL "Compile with -Wall/-W4 warning levels")

if (TANGLE_WALL_FLAGS)
  if (MSVC)
    set(ADDITIONAL_WALL_FLAGS
        /std:c++latest
        /permissive-
        /D_CRT_SECURE_NO_WARNINGS
        /W4
        /w14242
        /w14254
        /w14263
        /w14265
        /w14287
        /we4289
        /w14296
        /w14311
        /w14545
        /w14546
        /w14547
        /w14549
        /w14555
        /w14619
        /w14640
        /w14826
        /w14905
        /w14906
        /w14928
        /w14946)
  else()
    set(ADDITIONAL_WALL_FLAGS
        -std=c++17
        -Wall -Wextra
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
    )
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      list(APPEND ADDITIONAL_WALL_FLAGS -fcolor-diagnostics) # -Wlifetime
    else()
      list(APPEND ADDITIONAL_WALL_FLAGS
        -fdiagnostics-color
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
        )
    endif()
  endif()
endif()


set(TANGLE_TESTING ON CACHE BOOL "Compile and/or run self-tests")
set(TANGLE_DOCS OFF CACHE BOOL "Create doxygen documentation")
set(MODULES
	cookie
	uri
	msg
	cache
  nav
  http
)

foreach(MODULE ${MODULES})
	list(APPEND COVERALLS_DIRS ${MODULE}/include)
	list(APPEND COVERALLS_DIRS ${MODULE}/src)
endforeach()

include(Coveralls.cmake)

if (COVERALLS AND NOT TANGLE_TESTING)
	message(FATAL_ERROR "Coveralls are useless without -DTANGLE_TESTING=ON")
endif()

##################################################################
##  SOURCES
##################################################################

macro(WRAP_SOURCELIST)
	foreach (_file ${ARGN})
		get_filename_component(_basename ${_file} NAME_WE)
		get_filename_component(_path ${_file} PATH)

		string(REGEX REPLACE "/" "\\\\\\\\" _sourcegroup "${_path}")
		source_group("${_sourcegroup}" FILES ${_file})
	endforeach ()
endmacro()

if (MSVC)
file(GLOB NATVIS natvis/*.natvis)
add_definitions(/EHsc)
endif()

if(MSVC)
	add_definitions(/std:c++latest)
else()
	add_definitions(-std=c++17)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (TANGLE_TESTING)
	enable_testing()
	include( ${CMAKE_CURRENT_SOURCE_DIR}/googletest.cmake )
endif()

##################################################################
##  DOCUMENTATION
##################################################################

if (TANGLE_DOCS)
find_package( Doxygen )
if (DOXYGEN_FOUND)
	add_custom_target(doc)
	set_target_properties(doc PROPERTIES FOLDER Doxygen)
	if (MSVC)
		set(CMAKE_DOXYGEN_WARN_FORMAT "$file($line): $text")
	else()
		set(CMAKE_DOXYGEN_WARN_FORMAT "$file:$line: $text")
	endif()
else()
	message(STATUS "Doxy not found...")
endif()
endif()

##################################################################
##  SUBPROJECTS
##################################################################

foreach(MODULE ${MODULES})
	add_subdirectory( ${MODULE} )
endforeach()


##################################################################
##  PACKER
##################################################################

set(CPACK_PACKAGE_VENDOR "mbits-libs")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include( CPack )
