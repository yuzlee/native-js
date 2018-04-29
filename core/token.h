#pragma once

#include <string>
#include <memory>

#include "char_class.h"
#include "../util/noncopyable.h"
#include "../util/comparison.h"

using std::string;
using std::shared_ptr;
using std::make_shared;

namespace nativejs {
	namespace core {
		enum class TokenType {
			NOTHING = -1,
			EPSILON = 0,
			LITERAL = 1,

			L_PARENTHESES, // '('
			R_PARENTHESES, // ')'

			L_BRACKET, // '['
			R_BRACKET, // ']'

			L_BRACES, // '{'
			R_BRACES, // '}'

			CLOSURE, // '*'
			PLUS, // '+'
			COMPOSE, // '|'
			ALTER, // '?'

			END, // 'EOF'
			BLANK, // ' '
			ESCAPE, // '\'

			COMMA, // ','
			HYPHEN, // '-'
			CARET, // '^'

			EXTEND, // '\d\w'
			EXTEND_d, // \d
			EXTEND_D, // \D
			EXTEND_w, // \w
			EXTEND_W, // \W
			EXTEND_s, // \s
			EXTEND_S, // \S
			EXTEND_POINT // '.'

		};

		class TokenValue {
		public:
			typedef shared_ptr<TokenValue> Ptr;

		private:
			string value;

		public:
			TokenValue(const string& value) : value(value) {}

			//TokenValue(const string &&value) : value(std::move(value)) {}

			virtual bool operator==(const TokenValue &rhs) const noexcept;

			virtual bool operator!=(const TokenValue &rhs) const noexcept;

			virtual bool operator>(const TokenValue &rhs) const noexcept;

			virtual bool operator<(const TokenValue &rhs) const noexcept;

			virtual const string& to_string() const noexcept;
		};

		class CharClassTokenValue : public TokenValue {
		private:
			const CharClass& charClass;

		public:
			CharClassTokenValue(const string& name, const CharClass& charClass) : TokenValue(name), charClass(charClass) {}

			bool operator==(const CharClassTokenValue&rhs) const noexcept;

			bool operator!=(const CharClassTokenValue&rhs) const noexcept;

			bool operator>(const CharClassTokenValue&rhs) const noexcept;

			bool operator<(const CharClassTokenValue&rhs) const noexcept;

			const string& to_string() const noexcept;
		};

		class Token : noncopyable {
		public:
			typedef shared_ptr<Token> Ptr;
			typedef PtrLess<Token> Less;

		public:
			const TokenType type;
			TokenValue::Ptr value;

			explicit Token(TokenType type) : type(type), value(nullptr) {}
		public:
			static const Token epsilon;
			static const Token nothing;

			Token(TokenType type, const string& value) : type(type), value(make_shared<TokenValue>(value)) {}

			Token(TokenType type, TokenValue::Ptr value) : type(type), value(value) {}

			Token(const Token& rhs) : type(rhs.type), value(rhs.value) {}

			//Token &operator=(const Token &rhs) {
			//	this->type = rhs.type;
			//	this->value = rhs.value;
			//	return *this;
			//}

			bool operator==(const Token &rhs) const noexcept;

			bool operator!=(const Token &rhs) const noexcept;

			bool operator>(const Token &rhs) const noexcept;

			bool operator<(const Token &rhs) const noexcept;

			string type_string() const noexcept;

			string to_string() const noexcept;
		};
	}
}