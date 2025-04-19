cmake_minimum_required(VERSION 3.16)
project(HTTPserver)
set_property(GLOBAL PROPERTY libraries)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_VERBOSE_MAKEFILE ON)

# Find Threads Package
find_package(Threads REQUIRED)

add_subdirectory("simple_http_server")
add_subdirectory("src")





