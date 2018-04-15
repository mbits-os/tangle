cmake_minimum_required(VERSION 3.4)
project( tangle CXX )

set(TANGLE_VERSION_MAJOR 0)
set(TANGLE_VERSION_MINOR 1)
set(TANGLE_VERSION_PATCH 0)
set(TANGLE_VERSION "${TANGLE_VERSION_MAJOR}.${TANGLE_VERSION_MINOR}.${TANGLE_VERSION_PATCH}" )

set(TANGLE_TESTING ON CACHE BOOL "Compile and/or run self-tests")
set(TANGLE_DOCS OFF CACHE BOOL "Create doxygen documentation")
set(MODULES
	cstring
	cookie
	uri
	msg
	cache
	nav
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

set(CPACK_PACKAGE_VERSION_MAJOR "${TANGLE_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TANGLE_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TANGLE_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${TANGLE_VERSION}")
set(CPACK_PACKAGE_VENDOR "mbits-libs")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include( CPack )
