# ============================ Repo configurations =============================

# Project information.
cmake_minimum_required( VERSION 3.13.0 )
project( coreHTTP
         VERSION 1.0.0
         LANGUAGES C )

# Allow the project to be organized into folders.
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# Use C90.
set( CMAKE_C_STANDARD 90 )
set( CMAKE_C_STANDARD_REQUIRED ON )

# Do not allow in-source build.
if( ${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR} )
    message( FATAL_ERROR "In-source build is not allowed. Please build in a separate directory, such as ${PROJECT_SOURCE_DIR}/build." )
endif()

# Set global path variables.
get_filename_component(__root_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(ROOT_DIR ${__root_dir} CACHE INTERNAL "Source root.")

# We set the MODULES_DIR as the ROOT_DIR because the hub will include 
# httpFilePaths.cmake and will expect MODULES_DIR to be defined in that file.
set(MODULES_DIR "${ROOT_DIR}" CACHE INTERNAL "Modules root.")

# Configure options to always show in CMake GUI.
option( BUILD_TESTS
        "Set this to ON to build both demo and test executables. When OFF, only demo executables are built."
        OFF )
option( BUILD_CLONE_SUBMODULES
        "Set this to ON to automatically clone any required Git submodules. When OFF, submodules must be manually cloned."
        ON )

include( CMakeDependentOption )
CMAKE_DEPENDENT_OPTION( BUILD_SHARED_LIBS
                        "Set this to ON to build all libraries as shared libraries. When OFF, libraries build as static libraries."
                        ON "${ALLOW_SHARED_LIBRARIES}"
                        OFF )

# Set output directories.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# Unity test framework does not export the correct symbols for DLLs.
if( ${BUILD_TESTS} )
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

    include("${ROOT_DIR}/test/create_test.cmake")

    include_directories("test/CMock/vendor/unity/src/"
                        "test/CMock/vendor/unity/extras/fixture/src"
                        "test/CMock/vendor/unity/extras/memory/src"
                        "test/CMock/src"
            )
    link_directories("${CMAKE_BINARY_DIR}/lib"
            )

    # Add a target for running coverage on tests.
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/test/coverage.cmake
        DEPENDS cmock unity http_utest http_send_utest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    # Add the test subdirectory to build CMock
    add_subdirectory( test )
endif()

# ====================== coreHTTP library configurations =======================

# Include filepaths for source and include.
include(httpFilePaths.cmake)

# Add http_parser submodule to HTTP client library.
add_subdirectory( source/third_party )

# HTTP library target.
add_library( http
             ${HTTP_SOURCES} )

# HTTP public include path.
target_include_directories( http PUBLIC ${HTTP_INCLUDE_PUBLIC_DIRS}
                                        ${LOGGING_INCLUDE_DIRS} )

# HTTP private include path.
target_include_directories( http PRIVATE ${HTTP_INCLUDE_PRIVATE_DIRS} )

# Link http_parser to http target.
target_link_libraries( http PRIVATE http_parser )

# Organization of HTTP in IDE projects.
set_target_properties( http PROPERTIES FOLDER libraries/standard )
source_group( include FILES include/http_client.h )
source_group( src FILES ${HTTP_SOURCES} )
source_group( src\\private FILES src/private/http_internal.h )
