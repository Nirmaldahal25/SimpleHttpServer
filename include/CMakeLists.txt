function(add_libs_and_include_directories Lib)
add_library(${Lib} ${ARGN})
# message (${Lib})
# foreach(a IN LISTS ARGN)
#     message(${a})
# endforeach()
# message ("${CMAKE_CURRENT_SOURCE_DIR}/${Lib}")
include_directories(${Lib})
target_include_directories(${Lib} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${Lib}")
get_property(libss GLOBAL PROPERTY libraries)
set_property(GLOBAL PROPERTY libraries "${libss}" "${Lib}")
endfunction()

# add_libs_and_include_directories(Configuration
#                         AConfiguration/Settings.hpp
# )

add_libs_and_include_directories(Server 
                        Server/server.cpp
                        Server/server.hpp
)
add_libs_and_include_directories(Client
                        Client/client.hpp
                        Client/client.cpp
                        Client/threadpool.hpp
                        Client/threadpool.cpp              
)

add_libs_and_include_directories(Responses
                        Responses/responses.hpp
                        Responses/responses.cpp
)
add_libs_and_include_directories(Media_Class
                        Media_Class/Files.hpp
                        Media_Class/Files.cpp
)

add_libs_and_include_directories(Argparser
                        Argparser/argparse.hpp
                        Argparser/argparse.cpp
)

target_compile_features(Media_Class PRIVATE cxx_std_17)
