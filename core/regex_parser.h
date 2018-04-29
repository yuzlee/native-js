#pragma once;

#include <iostream>

#include "token.h"
#include "automata.hpp"

namespace nativejs {
	namespace core {
		class Exception {
		protected:
			static void _exit() {
				exit(1);
			}

		public:
			static void error(string msg) {
				std::cout << msg << std::endl;
				_exit();
			}
		};

		class LexerException : public Exception {
		public:
			static void error(string msg) {
				std::cout << "[Lexer Exception] " << msg << std::endl;
				_exit();
			}
		};

		class ParserException : public Exception {
		public:
			static void error(string msg) {
				std::cout << "[Parser Exception] " << msg << std::endl;
				_exit();
			}
		};

		class RegexLexer {
		private:
			const string text;

		public:
			RegexLexer(const string& text) :text(text) {
				// start

			}

			Token::Ptr& next() const {

			}
		};

		class RegexParser {
		private:
			const string text;
			RegexLexer lexer;
			Token::Ptr _next_token;

			void _consume() {
				this->_next_token = this->lexer.next();
			}

			void _match(const TokenType type) {
				if (this->_next_token->type == type) {
					this->_consume();
				} else {
					LexerException::error("mismatched token: " + this->_next_token->value->to_string());
				}
			}

			NFA _expression() {
				auto expr_nfa = this->_expression_without_compose();
				if (this->_next_token->type == TokenType::COMPOSE) {
					this->_match(TokenType::COMPOSE);
					return std::move(expr_nfa | this->_expression());
				}
				return expr_nfa;
			}

			NFA _expression_without_compose() {
				auto factor_nfa = this->_factor();
				auto type = this->_next_token->type;
				if (type == TokenType::LITERAL ||
					type == TokenType::L_PARENTHESES || // (
					type == TokenType::L_BRACKET || // [
					type > TokenType::EXTEND) {
					auto nfa = this->_expression_without_compose();
					return std::move(factor_nfa + nfa);
				}
				return std::move(factor_nfa);
			}

			// factor -> (term)+ | (term)* | (term)? | (term){0,} | (term){0,1} | (term){1,}
			NFA _factor() {
				auto term_nfa = this->_term();
				switch (this->_next_token->type) {
				case TokenType::PLUS: { // +
					term_nfa++;
					this->_match(TokenType::PLUS);
					return std::move(term_nfa);
				}
				case TokenType::CLOSURE: { // *
					auto nfa = ~(term_nfa++);
					this->_match(TokenType::CLOSURE);
					return std::move(nfa);
				}
				case TokenType::ALTER: { // ?
					auto nfa = ~term_nfa;
					this->_match(TokenType::ALTER);
					return std::move(nfa);
				}
				case TokenType::L_BRACES: { // {} pattern
					this->_match(TokenType::L_BRACES);
					// TODO {0,1} {1,} {0,}
					this->_match(TokenType::LITERAL);
					auto _start = std::atoi(this->_next_token->value->to_string().c_str());
					if (_start != 0 || _start != 1) {
						ParserException::error("invalid repeat pattern: start with " + _start);
					}
					this->_match(TokenType::LITERAL);
					if (this->_next_token->value->to_string() != ",") {
						ParserException::error("unexpected {} pattern");
					}
					this->_consume();
					if (this->_next_token->type != TokenType::R_BRACES) {
						this->_match(TokenType::LITERAL);
						auto _end = std::atoi(this->_next_token->value->to_string().c_str());
						if (_start == 0 && _end == 1) {  // {0,1}
							// the same as ALTER
							auto nfa = ~term_nfa;
							this->_match(TokenType::ALTER);
							return std::move(nfa);
						}
						this->_match(TokenType::R_BRACES);
					} else {
						if (_start == 0) { // {0,}
							// the same as CLOSURE
							auto nfa = ~(term_nfa++);
							this->_match(TokenType::CLOSURE);
							return std::move(nfa);
						} else if (_start == 1) { // {1,}
							// the same as PLUS
							term_nfa++;
							this->_match(TokenType::PLUS);
							return std::move(term_nfa);
						}
					}
				}
				default:
					ParserException::error("invalid repeat pattern: " + this->_next_token->value->to_string());
				}
			}

			// extend -> x-y | ^x-y
			NFA _extend_expression() {
				vector<function<bool(const CharClass::CharType)>> func_vector;
				auto revert = false;
				this->_consume();
				switch (this->_next_token->type) {
				case TokenType::LITERAL: {
					while (true) {
						auto _start = this->_next_token->value->to_string()[0];
						this->_consume();
						switch (this->_next_token->type) {
						case TokenType::HYPHEN: { // x-y
							this->_match(TokenType::LITERAL);
							auto _end = this->_next_token->value->to_string()[0];
							func_vector.push_back([_start, _end](const CharClass::CharType& _t) {
								return _t >= _start && _t <= _end;
							});
						}
						case TokenType::LITERAL: { // y
							func_vector.push_back([_start](const CharClass::CharType& _t) {
								return _t == _start;
							});
							continue;
						}
						case TokenType::R_BRACKET: {
							goto _EXTEND_RETURN;
						}
						default:
							ParserException::error("invalid extend pattern: " + this->_next_token->value->to_string());
						}
					}
				}
				case TokenType::CARET: {
					revert = true;
					this->_match(TokenType::LITERAL);
				}
				default:
					ParserException::error("invalid extend pattern: " + this->_next_token->value->to_string());
				}

			_EXTEND_RETURN:
				auto nfa = NFA(Token(TokenType::EXTEND, make_shared<CharClassTokenValue>(CharClass([func_vector, revert](const CharClass::CharType& _t) {
					auto result = false;
					for (auto& func : func_vector) {
						result = result || func(_t); // concat all conditions
					}
					if (revert) {
						return !result;
					}
					return result; // (y & x) | (^y & ^x)
				}))));
				return std::move(nfa);
			}

			// term -> (expr) | literal | [extend]
			NFA _term() {
				switch (this->_next_token->type) {
				case TokenType::LITERAL: { // literal
					auto nfa = NFA(*this->_next_token);
					this->_match(TokenType::LITERAL);
					return std::move(nfa);
				}
				case TokenType::L_PARENTHESES: { // ()
					this->_match(TokenType::L_PARENTHESES);
					auto nfa = this->_expression();
					this->_match(TokenType::R_PARENTHESES);
					return std::move(nfa);
				}
				case TokenType::L_BRACKET: { // []
					this->_match(TokenType::L_BRACKET);
					auto nfa = this->_extend_expression();
					this->_match(TokenType::R_BRACKET);
					return std::move(nfa);
				}
				case TokenType::EXTEND_d: {
					this->_match(TokenType::EXTEND_d);
					auto nfa = NFA(Token(TokenType::EXTEND_d, make_shared<CharClassTokenValue>(CharClass::Common::number)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_D: {
					this->_match(TokenType::EXTEND_D);
					auto nfa = NFA(Token(TokenType::EXTEND_D, make_shared<CharClassTokenValue>(CharClass::Common::not_number)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_w: {
					this->_match(TokenType::EXTEND_w);
					auto nfa = NFA(Token(TokenType::EXTEND_w, make_shared<CharClassTokenValue>(CharClass::Common::ascii_char)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_W: {
					this->_match(TokenType::EXTEND_W);
					auto nfa = NFA(Token(TokenType::EXTEND_W, make_shared<CharClassTokenValue>(CharClass::Common::not_ascii_char)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_s: {
					this->_match(TokenType::EXTEND_s);
					auto nfa = NFA(Token(TokenType::EXTEND_s, make_shared<CharClassTokenValue>(CharClass::Common::blank)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_S: {
					this->_match(TokenType::EXTEND_S);
					auto nfa = NFA(Token(TokenType::EXTEND_S, make_shared<CharClassTokenValue>(CharClass::Common::not_blank)));
					return std::move(nfa);
				}
				case TokenType::EXTEND_POINT: {
					this->_match(TokenType::EXTEND_POINT);
					auto nfa = NFA(Token(TokenType::EXTEND_POINT, make_shared<CharClassTokenValue>(CharClass::Common::not_new_line)));
					return std::move(nfa);
				}
				default:
					break;
				}
			}

		public:
			RegexParser(const string& text) : text(text), lexer(text), _next_token(nullptr) {}

			NFA generate_nfa() {
				return std::move(this->_expression());
			}
		};
	}
}