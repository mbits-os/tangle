add_definitions(${ADDITIONAL_WALL_FLAGS})

add_library(lookup
    lib/file.cc
    lib/lookup.cc
    lib/str.cc
    lib/json.cc
)
target_include_directories(lookup PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(dtd
    dtd/dtd_database.cc
    dtd/dtd_parser.cc
    dtd/dtd_types.cc
)
target_include_directories(dtd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dtd)
target_link_libraries(dtd lookup)

add_executable(html-dtd apps/html-dtd.cc)
target_link_libraries(html-dtd dtd)

add_executable(html-entities apps/html-entities.cc)
target_link_libraries(html-entities lookup)

set_target_properties(lookup dtd html-dtd html-entities PROPERTIES
	FOLDER tangle/tools)
