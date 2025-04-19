# set(external stdc++fs ${external})
add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra) 
target_link_libraries(${PROJECT_NAME} PUBLIC  simple_http_server  ${CMAKE_THREAD_LIBS_INIT} -lstdc++fs)

