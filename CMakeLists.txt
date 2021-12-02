cmake_minimum_required(VERSION 3.16)
project(HTTPserver)
set_property(GLOBAL PROPERTY libraries)
#add_subdirectory("URLS")
#add_subdirectory("VIEWS")
# set(CXX_STANDARD ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory("include")
add_subdirectory("src")




