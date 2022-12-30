function(add_tangle_library NAME)
    add_library(${NAME} STATIC ${ARGN})
    set_target_properties(${NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        FOLDER tangle)
    target_include_directories(${NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    target_compile_options(${NAME} PRIVATE ${ADDITIONAL_WALL_FLAGS})
    if (TANGLE_INSTALL)
        install(TARGETS ${NAME}
            EXPORT tangle
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin)
        install(DIRECTORY include/tangle/ DESTINATION include/tangle)
    endif()
endfunction()