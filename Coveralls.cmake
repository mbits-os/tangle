option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

find_package(PythonInterp)

if (COVERALLS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
	set(COVERALLS_FILE ${PROJECT_BINARY_DIR}/coveralls.json)

	if (NOT COVERALLS_DIRS)
		message(FATAL_ERROR "COVERALLS_DIRS not set. Aborting")
	endif()

	find_program(GCOV_EXECUTABLE gcov)
	if (NOT GCOV_EXECUTABLE)
		message(FATAL_ERROR "gcov not found! Aborting...")
	endif()

	find_package(Git)

	if (UNIX)
	add_custom_target(coveralls_prepare
		# Reset all counters
		COMMAND find -type f -name '*.gcda' -exec rm {} '\;'
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Preparing for coverage..."
	)
	else()
		add_custom_target(coveralls_prepare)
	endif()

	add_custom_target(coveralls_test
		# Run tests and regenerate the counters
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
		DEPENDS coveralls_prepare
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Running all tests..."
	)

	set(JOIN_DIRS "")
	foreach(DIR_NAME ${COVERALLS_DIRS})
		if (JOIN_DIRS)
			set(JOIN_DIRS "${JOIN_DIRS}:")
		endif()
		set(JOIN_DIRS "${JOIN_DIRS}${DIR_NAME}")
	endforeach()

	message(STATUS "PYTHON_EXECUTABLE is: ${PYTHON_EXECUTABLE}")
	add_custom_target(coveralls_generate
		COMMAND rm -rf cover-data
		COMMAND mkdir -p cover-data
		COMMAND ${PYTHON_EXECUTABLE}
			"${PROJECT_SOURCE_DIR}/coveralls.py"
			--tool "${GCOV_EXECUTABLE}"
			--git "${GIT_EXECUTABLE}"
			--src_dir "${PROJECT_SOURCE_DIR}"
			--bin_dir "${PROJECT_BINARY_DIR}"
			--int_dir "${PROJECT_BINARY_DIR}/cover-data"
			--dirs "${JOIN_DIRS}"
			--out "${COVERALLS_FILE}"
		DEPENDS
			coveralls_test
			"${PROJECT_SOURCE_DIR}/coveralls.py"
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Generating coveralls output..."
	)

	if (COVERALLS_UPLOAD)
		message("COVERALLS UPLOAD: ON")

		find_program(CURL_EXECUTABLE curl)

		if (NOT CURL_EXECUTABLE)
			message(FATAL_ERROR "Coveralls: curl not found! Aborting")
		endif()

		add_custom_target(coveralls_upload
			# Upload the JSON to coveralls.
			COMMAND ${CURL_EXECUTABLE} -S -F "json_file=@${COVERALLS_FILE}" https://coveralls.io/api/v1/jobs

			DEPENDS coveralls_generate

			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
			COMMENT "Uploading coveralls output...")

		add_custom_target(coveralls DEPENDS coveralls_upload)
	else()
		message("COVERALLS UPLOAD: OFF")
		add_custom_target(coveralls DEPENDS coveralls_generate)
	endif()
endif()
