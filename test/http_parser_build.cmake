macro( clone_http_parser )
        find_package( Git REQUIRED )
        message( "Cloning submodule http_parser." )
        execute_process( COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${HTTP_PARSER_DIR}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE HTTP_PARSER_CLONE_RESULT )

        if( NOT ${HTTP_PARSER_CLONE_RESULT} STREQUAL "0" )
            message( FATAL_ERROR "Failed to clone http_parser submodule." )
        endif()
endmacro()

# http_parser library target.
add_library( http_parser
             ${HTTP_PARSER_DIR}/http_parser.c )

# http_parser public include path.
target_include_directories( http_parser PUBLIC
                                        ${HTTP_PARSER_DIR} )
