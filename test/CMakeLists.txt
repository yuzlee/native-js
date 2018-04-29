# CMakeList.txt : CMake project for NativeJS, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.3)

add_definitions(-std=c++11)

# Add source to this project's executable.
add_executable (state_test
		"../core/char_class.h"
		"../core/char_class.cpp"
        "../core/state.h"
        "../core/state.cpp"
        "state_test.cpp")

add_executable (token_test
		"../core/char_class.h"
		"../core/char_class.cpp"
        "../core/token.h"
        "../core/token.cpp"
        "token_test.cpp")

add_executable (automata_test
		"../core/char_class.h"
		"../core/char_class.cpp"
		"../core/token.h"
        "../core/token.cpp"
        "../core/state.h"
        "../core/state.cpp"
        "automata_test.cpp")

# TODO: Add tests and install targets if needed.
