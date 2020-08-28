include("${ROOT_DIR}/httpFilePaths.cmake")
project ("http unit test")
cmake_minimum_required (VERSION 3.13)

# ============= Build Configuration for CMock and Unity libraries. =============

add_library(cmock STATIC
        "${ROOT_DIR}/test/CMock/src/cmock.c"
        )

set_target_properties(cmock PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        COMPILE_FLAGS "-Og"
        )

add_library(unity STATIC
        "${ROOT_DIR}/test/CMock/vendor/unity/src/unity.c"
        "${ROOT_DIR}/test/CMock/vendor/unity/extras/fixture/src/unity_fixture.c"
        "${ROOT_DIR}/test/CMock/vendor/unity/extras/memory/src/unity_memory.c"
        )
set_target_properties(unity PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        )

target_include_directories(cmock PUBLIC
        ${ROOT_DIR}/test/CMock/src
        ${ROOT_DIR}/test/CMock/vendor/unity/src/
        ${ROOT_DIR}/test/CMock/examples
        )

target_link_libraries(cmock unity)

# ========================= Unit test configurations ===========================

# ====================  Define your project name (edit) ========================
set(project_name "http")

# =====================  Create your mock here  (edit)  ========================

# list the files to mock here
list(APPEND mock_list
            ${MODULES_DIR}/source/third_party/http_parser/http_parser.h
        )
# list the directories your mocks need
list(APPEND mock_include_list
             ${MODULES_DIR}/source/third_party/http_parser
        )
#list the definitions of your mocks to control what to be included
list(APPEND mock_define_list
            ""
       )

# ================= Create the library under test here (edit) ==================

# list the files you would like to test here
list(APPEND real_source_files
            ${HTTP_SOURCES}
        )
# list the directories the module under test includes
list(APPEND real_include_directories
            .
            ${HTTP_INCLUDE_PUBLIC_DIRS}
            ${HTTP_INCLUDE_PRIVATE_DIRS}
        )

# =====================  Create UnitTest Code here (edit)  =====================

# list the directories your test needs to include
list(APPEND test_include_directories
            .
            ${HTTP_INCLUDE_PUBLIC_DIRS}
            ${HTTP_INCLUDE_PRIVATE_DIRS}
            ${HTTP_TEST_INCLUDE_DIRS}
        )

# =============================  (end edit)  ===================================

set(mock_name "http_parser_mock")
set(real_name "${project_name}_real")

create_mock_list(${mock_name}
                "${mock_list}"
                "${ROOT_DIR}/test/project.yml"
                "${mock_include_list}"
                "${mock_define_list}"
        )

create_real_library(${real_name}
                    "${real_source_files}"
                    "${real_include_directories}"
                    "${mock_name}"
        )

list(APPEND utest_link_list
            lib${real_name}.a
            -l${mock_name}
        )

list(APPEND utest_dep_list
            ${real_name}
        )

set(utest_name "${project_name}_utest")
set(utest_source "${project_name}_utest.c")
create_test(${utest_name}
            ${utest_source}
            "${utest_link_list}"
            "${utest_dep_list}"
            "${test_include_directories}"
        )

set(utest_name "${project_name}_send_utest")
set(utest_source "${project_name}_send_utest.c")
create_test(${utest_name}
            ${utest_source}
            "${utest_link_list}"
            "${utest_dep_list}"
            "${test_include_directories}"
        )
