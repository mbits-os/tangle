cmake_minimum_required(VERSION 3.15)
project(tangle VERSION 0.1.0 LANGUAGES CXX)

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  message(STATUS "Building for C++${CMAKE_CXX_STANDARD}")
else()
  message(STATUS "Building for C++${CMAKE_CXX_STANDARD} (set externally)")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(tangle_library)

if (NOT TARGET CURL::libcurl)
find_package(CURL REQUIRED CONFIG)
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(TANGLE_WALL_FLAGS ON CACHE BOOL "Compile with -Wall/-W4 warning levels")

if (TANGLE_WALL_FLAGS)
  if (MSVC)
    set(ADDITIONAL_WALL_FLAGS
        /permissive-
        /Zc:__cplusplus
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
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
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
set(TANGLE_INSTALL ON CACHE BOOL "Create install target(s)")
set(TANGLE_DOCS OFF CACHE BOOL "Create doxygen documentation")
set(MODULES
  str
	cookie
	uri
	msg
  nav
  browser
)

set(EXTRAS
  nav/extra/proto-curl
)

if (TANGLE_TESTING)
  set(COVERALLS_PREFIX TANGLE_)
  foreach(MODULE ${MODULES})
    list(APPEND TANGLE_COVERALLS_DIRS ${MODULE}/include ${MODULE}/src)
  endforeach()
  foreach(MODULE ${EXTRAS})
    list(APPEND TANGLE_COVERALLS_DIRS ${MODULE}/include ${MODULE}/src)
  endforeach()

  set(TANGLE_COVERALLS_IGNORE_FILES
    # The file introduces almost 7000 lines, which more than quadruples
    # number of total lines; this clouds the reports a little. To double
    # check the coverage, comment next line.
    browser/src/entities.cpp

    # Another auto-generated file, this one adds 360 lines total.
    browser/src/html_dtd.cpp
  )

  include(tools/coveralls/Coveralls.cmake)
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

set(TANGLE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
function(add_natvis PROJECT FILENAME)
  if (MSVC)
    target_sources(${PROJECT} PUBLIC
        $<BUILD_INTERFACE:${TANGLE_ROOT}/natvis/${FILENAME}.natvis>
        $<INSTALL_INTERFACE:include/tangle/natvis/${FILENAME}.natvis>
    )
  endif()
endfunction()

if (MSVC)
add_definitions(/EHsc)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (TANGLE_TESTING)
  enable_testing()
  if (NOT TARGET GTest::gtest)
    find_package(GTest REQUIRED CONFIG HINTS ${PROJECT_BINARY_DIR})
  endif()
endif()

add_subdirectory(tools/lookup)

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

foreach(MODULE ${EXTRAS})
	add_subdirectory( ${MODULE} )
endforeach()

if (TANGLE_INSTALL)
  install(EXPORT tangle NAMESPACE "tangle::" DESTINATION lib/cmake)
  if (MSVC)
    install(DIRECTORY natvis/ DESTINATION include/tangle/natvis)
  endif()
endif()

##################################################################
##  PACKER
##################################################################

set(CPACK_PACKAGE_VENDOR "mbits-libs")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include( CPack )
