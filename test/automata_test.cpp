#include <iostream>
#include <ctime>

#include "../core/token.h"
#include "../core/state.h"
#include "../core/automata.hpp"

using namespace std;
using namespace nativejs::core;

int main() {
	Token _a(TokenType::LITERAL, "a");
	Token _b(TokenType::LITERAL, "b");
	NFA a1(_a), a2(_a), a3(_a);
	NFA b1(_b), b2(_b), b3(_b), b4(_b);

	std::cout << "start parsing" << std::endl;
	auto _start = std::clock();

	// (a|b)*abb
	auto re = (~((a1 | b1)++)) + a2 + b2 + b3; // +(~((a3 | b4)++));

	auto states = re.subset_construct();

	std::cout << "time used: " << (((double)(std::clock() - _start))) << "ms" << std::endl;
	//for (auto& state: states) {
	//	std::cout << state << std::endl;
	//}

	return 0;
}