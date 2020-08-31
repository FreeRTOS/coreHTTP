# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# HTTP library source files.
set( HTTP_SOURCES
     ${CMAKE_CURRENT_LIST_DIR}/source/http_client.c )

# HTTP library Public Include directories.
set( HTTP_INCLUDE_PUBLIC_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/source/include
     ${CMAKE_CURRENT_LIST_DIR}/source/portable )

# HTTP library Private Include directories.
set( HTTP_INCLUDE_PRIVATE_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/source
     ${HTTP_PARSER_DIR} )

# HTTP library Include directories for Tests.
set( HTTP_INCLUDE_THIRD_PARTY_DIRS
     ${HTTP_PARSER_DIR} )
