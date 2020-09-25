# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# HTTP library source files.
set( HTTP_SOURCES
     ${CMAKE_CURRENT_LIST_DIR}/source/core_http_client.c )

# HTTP library public include directories.
set( HTTP_INCLUDE_PUBLIC_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/source/include
     ${CMAKE_CURRENT_LIST_DIR}/source/portable )

# HTTP library private include directories.
set( HTTP_INCLUDE_PRIVATE_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/source/third-party/http_parser )

# HTTP library private sources.
set( HTTP_PRIVATE_SOURCES
     ${CMAKE_CURRENT_LIST_DIR}/source/third-party/http_parser/http_parser.c )
