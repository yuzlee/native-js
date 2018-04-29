#include <iostream>
#include <string>
#include <set>

#include "../core/token.h"

using namespace std;
using namespace nativejs::core;

void print(const Token& token) {
	std::cout << token.to_string() << std::endl;
}

int main() {
	auto token1 = Token(TokenType::EPSILON, make_shared<TokenValue>("test1"));

	auto token2 = Token(TokenType::EPSILON, "test1");

	print(token1);
	print(token2);

	set<Token, Less<Token>> _set{ token1, token2, Token(TokenType::LITERAL, "123") };

	std::cout << "size: " << _set.size() << std::endl;

	std::cout << "eq: " << (token1 == token2) << std::endl;
	std::cout << "ne: " << (token1 != token2) << std::endl;
	std::cout << "t1 < t2: " << (token1 < token2) << std::endl;
	std::cout << "t2 < t1: " << (token2 < token1) << std::endl;

	print(Token::epsilon);
	print(Token::nothing);

	return 0;
}