# ============================ Repo configurations =============================

# Project information.
cmake_minimum_required( VERSION 3.13.0 )
project( coreHTTP
         VERSION 4.0.2
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
get_filename_component(__MODULE_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(MODULE_ROOT_DIR ${__MODULE_ROOT_DIR} CACHE INTERNAL "HTTP source root.")

# Configure options to always show in CMake GUI.
option( BUILD_CLONE_SUBMODULES
        "Set this to ON to automatically clone any required Git submodules. When OFF, submodules must be manually cloned."
        ON )

include( CMakeDependentOption )
CMAKE_DEPENDENT_OPTION( BUILD_SHARED_LIBS
                        "Set this to ON to build HTTP as a shared library. When OFF, HTTP builds as a static library."
                        ON "${ALLOW_SHARED_LIBRARIES}"
                        OFF )

# Set output directories.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# ====================== coreHTTP library configurations =======================

# Include filepaths for source and include.
include(httpFilePaths.cmake)

# Add http_parser submodule to HTTP client library.
add_subdirectory( source/third_party )

# HTTP library target.
add_library( http
             ${HTTP_SOURCES} )

# HTTP public include path.
target_include_directories( http PUBLIC ${HTTP_INCLUDE_PUBLIC_DIRS} )

# HTTP private include path. The unit test is default build so we include the test
# configuration for now.
target_include_directories( http PRIVATE ${HTTP_INCLUDE_PRIVATE_DIRS}
                            ${MODULE_ROOT_DIR}/test )

# Link http_parser to http target.
target_link_libraries( http PRIVATE http_parser )

# Organization of HTTP in IDE projects.
set_target_properties( http PROPERTIES FOLDER libraries/standard )
source_group( include FILES source/include/http_client.h source/portable/transport_interface.h )
source_group( source FILES ${HTTP_SOURCES} )
source_group( source\\private FILES source/private/http_internal.h )

# ============================ Test configurations =============================

# Include CMock build configuration.
include( test/cmock_build.cmake )

# Check if the CMock source directory exists, and if not present, clone the submodule
# if BUILD_CLONE_SUBMODULES configuration is enabled. 
if( NOT EXISTS ${MODULE_ROOT_DIR}/test/CMock/src )
    # Attempt to clone CMock.
    if( ${BUILD_CLONE_SUBMODULES} )
        clone_cmock()
    else()
        message( FATAL_ERROR "The required submodule CMock does not exist. Either clone it manually, or set BUILD_CLONE_SUBMODULES to 1 to automatically clone it during build." )
    endif()
endif()

# Add unit test and coverage configuration only if CMock directory exists.
if( EXISTS ${MODULE_ROOT_DIR}/test/CMock/src )

    # Use CTest utility for managing test runs. This has to be added BEFORE
    # defining test targets with add_test()
    enable_testing()

    # Add build targets for CMock and Unit, required for unit testing.
    add_cmock_targets()

    # Add function to enable CMOck based tests and coverage.
    include("${MODULE_ROOT_DIR}/tools/cmock/create_test.cmake")

    # Include build configuration for unit tests.
    add_subdirectory( test )
    
    # Add a target for running coverage on tests.
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/cmock/coverage.cmake
        DEPENDS cmock unity http_utest http_send_utest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
