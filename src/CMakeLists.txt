get_property(libsss GLOBAL PROPERTY libraries)
# set(external stdc++fs ${external})
find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC  ${libsss} ${CMAKE_THREAD_LIBS_INIT})

# set(ConfigFiles static
#                 templates)

# foreach(config ${ConfigFiles})

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
#         copy_directory "${CMAKE_SOURCE_DIR}/${config}" $<TARGET_FILE_DIR:${PROJECT_NAME}>"/${config}")
# endforeach()
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra) 
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# set(CMAKE_CXX_FLAGS "-v")