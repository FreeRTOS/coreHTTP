# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# HTTP library source files.
set( HTTP_SOURCES
     ${MODULE_ROOT_DIR}/source/http_client.c )

# HTTP library Public Include directories.
set( HTTP_INCLUDE_PUBLIC_DIRS
     ${MODULE_ROOT_DIR}/source/include
     ${MODULE_ROOT_DIR}/source/portable )

# HTTP library Private Include directories.
set( HTTP_INCLUDE_PRIVATE_DIRS
     ${MODULE_ROOT_DIR}/source
     ${MODULE_ROOT_DIR}/source/third_party/http_parser )

# HTTP library Include directories for Tests.
set( HTTP_INCLUDE_THIRD_PARTY_DIRS
     ${MODULE_ROOT_DIR}/source/third_party/http_parser )
