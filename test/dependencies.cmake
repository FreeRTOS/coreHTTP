# Unity test framework does not export the correct symbols for DLLs.
set( ALLOW_SHARED_LIBRARIES ON )

# Configuration for CMock if testing is enabled.
# Check if the CMock source directory exists.
if( NOT EXISTS ${ROOT_DIR}/test/CMock/src )
    # Attempt to clone CMock.
    if( ${BUILD_CLONE_SUBMODULES} )
        find_package( Git REQUIRED )

        message( "Cloning submodule CMock." )
        execute_process( COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive test/CMock
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                            RESULT_VARIABLE CMOCK_CLONE_RESULT )

        if( NOT ${CMOCK_CLONE_RESULT} STREQUAL "0" )
            message( FATAL_ERROR "Failed to clone CMock submodule." )
        endif()
    else()
        message( FATAL_ERROR "The required submodule CMock does not exist. Either clone it manually, or set BUILD_CLONE_SUBMODULES to 1 to automatically clone it during build." )
    endif()
endif()

include("${ROOT_DIR}/tools/cmock/create_test.cmake")

include_directories("test/CMock/vendor/unity/src/"
                    "test/CMock/vendor/unity/extras/fixture/src"
                    "test/CMock/vendor/unity/extras/memory/src"
                    "test/CMock/src"
        )
link_directories("${CMAKE_BINARY_DIR}/lib"
        )

# Add a target for running coverage on tests.
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/cmock/coverage.cmake
    DEPENDS cmock unity http_utest http_send_utest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Add the test subdirectory to build CMock
add_subdirectory( test )
