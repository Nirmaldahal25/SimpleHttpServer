function(add_libs_and_include_directories Lib)
add_library(${Lib} ${ARGN})
set(LIB_INCLUDE_DIRECTORY_PATH "${CMAKE_CURRENT_BINARY_DIR}/include")
make_directory(${LIB_INCLUDE_DIRECTORY_PATH})
# message (${Lib})
foreach(library_file IN LISTS ARGN)
    get_filename_component(FILE_EXTENSION ${library_file} EXT)
    if(FILE_EXTENSION STREQUAL ".cc" OR FILE_EXTENSION STREQUAL ".hpp" OR  FILE_EXTENSION STREQUAL ".h")
        get_filename_component(DIR_PATH "${library_file}" DIRECTORY)
        string(REPLACE "/" " " DIR_PARTS "${DIR_PATH}") 
        SET(FOLDER_DIR " ") 
        foreach(FOLDER ${DIR_PARTS})
            if(${FOLDER_DIR} STREQUAL " ")
                SET(FOLDER_DIR ${FOLDER})
            else()
                SET(FOLDER_DIR ${FOLDER_DIR}/${FOLDER})
            endif()
            make_directory(${LIB_INCLUDE_DIRECTORY_PATH}/${FOLDER_DIR})
        endforeach()

        file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${library_file} "${LIB_INCLUDE_DIRECTORY_PATH}/${library_file}")
    endif()
endforeach()

message(${LIB_INCLUDE_DIRECTORY_PATH})
target_include_directories(${Lib} PUBLIC ${LIB_INCLUDE_DIRECTORY_PATH})
endfunction()


add_libs_and_include_directories(simple_http_server Server/server.cpp
                                                    Server/server.hpp
                                                    Client/client.hpp
                                                    Client/client.cpp
                                                    Client/threadpool.hpp
                                                    Client/threadpool.cpp  
                                                    Responses/responses.hpp
                                                    Responses/responses.cpp
                                                    HTTPMessages/httprequests.hpp
                                                    HTTPMessages/httprequests.cpp
                                                    HTTPMessages/httpresponses.cpp
                                                    Files/files.hpp
                                                    Files/files.cpp
)   
                        
add_libs_and_include_directories(Argparser  Argparser/argparse.hpp
                                            Argparser/argparse.cpp
)

# Compile library with C++ 17 standard
target_compile_features(simple_http_server PRIVATE cxx_std_17)
target_compile_features(Argparser PRIVATE cxx_std_17)

# Copy Templates Folder to Current Binary Dir
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/templates" POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/templates"
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/templates" "${CMAKE_BINARY_DIR}/templates"
      COMMENT "Copying Templates Directory"
)
add_custom_target(copy_template ALL DEPENDS "${CMAKE_BINARY_DIR}/templates")

# TEMPLATES_DIR is visible only inside the simple_http_server library only
target_compile_definitions(simple_http_server PRIVATE TEMPLATES_DIR=${CMAKE_BINARY_DIR}/templates)
add_dependencies(simple_http_server copy_template)