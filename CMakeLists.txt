# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.3)

add_definitions(-std=c++11)

project ("NativeJS")

# Include sub-projects.
add_subdirectory("test" EXCLUDE_FROM_ALL "test/build")

add_subdirectory ("core")
