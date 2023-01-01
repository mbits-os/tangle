set(TANGLE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (TANGLE_SINGLE_LIB)
    add_library(tangle STATIC dummy.cc)
    set_target_properties(tangle PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        FOLDER tangle)
    target_include_directories(tangle PUBLIC $<INSTALL_INTERFACE:include>)
    target_compile_options(tangle PRIVATE ${ADDITIONAL_WALL_FLAGS})

    function(add_tangle_library NAME)
        set(_SRCS)
        foreach(SOURCE ${ARGN})
            list(APPEND _SRCS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
        endforeach()
        target_sources(tangle PRIVATE ${_SRCS})
        target_include_directories(tangle PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )

        add_library(${NAME}_target INTERFACE)
        target_link_libraries(${NAME}_target INTERFACE tangle)
    endfunction()

    function(tangle_link_libraries NAME)
    endfunction()
else()
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

        add_library(${NAME}_target INTERFACE)
        target_link_libraries(${NAME}_target INTERFACE ${NAME})
    endfunction()

    function(tangle_link_libraries NAME)
        target_link_libraries(${NAME} PUBLIC ${ARGN})
    endfunction()
endif()

function(add_natvis PROJECT FILENAME)
    if (MSVC)
        target_sources(${PROJECT}_target INTERFACE
            $<BUILD_INTERFACE:${TANGLE_ROOT}/natvis/${FILENAME}.natvis>
            $<INSTALL_INTERFACE:include/tangle/natvis/${FILENAME}.natvis>
        )
    endif()
endfunction()