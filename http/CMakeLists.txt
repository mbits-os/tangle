cmake_minimum_required(VERSION 3.10)
project(http VERSION 0.1.0 LANGUAGES CXX )

add_library(http STATIC
	src/curl.cc
	include/tangle/http/proto.hh
)
set_target_properties(http PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    FOLDER libs
)
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(http PUBLIC nav CURL::libcurl)

install(TARGETS http
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)
install(DIRECTORY include/tangle/ DESTINATION include/tangle)
