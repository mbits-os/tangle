cmake_minimum_required(VERSION 3.10)
project(http VERSION 0.1.0 LANGUAGES CXX )

set(SOURCES
	src/client.cpp
	src/curl.cpp
	src/headers.cpp
	include/tangle/http/client.hpp
	include/tangle/http/headers.hpp
	include/tangle/http/doc_impl.hpp
	include/tangle/http/proto.hpp
)

WRAP_SOURCELIST(${SOURCES})

if (MSVC)
	list(APPEND SOURCES ../natvis/header.natvis)
endif()

add_library(http STATIC ${SOURCES})
set_target_properties(http PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    FOLDER libs
)
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(http PUBLIC nav CURL::libcurl)
target_compile_options(http PRIVATE ${ADDITIONAL_WALL_FLAGS})

if (TANGLE_INSTALL)
install(TARGETS http
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
endif()
