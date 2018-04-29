# CMakeList.txt : CMake project for NativeJS, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.3)

add_definitions(-std=c++11)

# Add source to this project's executable.
# add_executable (NativeJS "NativeJS.cpp" "NativeJS.h")

# TODO: Add tests and install targets if needed.
SET(CORE_SRC
        state.h
        state.cpp
        token.h
		token.cpp
		char_class.h
		char_class.cpp
		regex_parser.h
		regex_parser.cpp
        automata.hpp
        test.cpp)

add_executable(core ${CORE_SRC})
