cmake_minimum_required(VERSION 3.16)
project(HTTPserver)
set_property(GLOBAL PROPERTY libraries)
#add_subdirectory("URLS")
#add_subdirectory("VIEWS")
# set(CXX_STANDARD ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory("include")
add_subdirectory("src")




