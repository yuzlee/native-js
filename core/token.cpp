#include "token.h"

namespace nativejs {
	namespace core {
		const Token Token::epsilon = Token(TokenType::EPSILON, "epsilon");
		const Token Token::nothing = Token(TokenType::NOTHING, "nothing");

		// ****************** TokenValue ****************************
		bool TokenValue::operator==(const TokenValue &rhs) const noexcept {
			return this->value == rhs.value;
		}

		bool TokenValue::operator!=(const TokenValue &rhs) const noexcept {
			return this->value != rhs.value;
		}

		bool TokenValue::operator>(const TokenValue &rhs) const noexcept {
			return this->value > rhs.value;
		}

		bool TokenValue::operator<(const TokenValue &rhs) const noexcept {
			return this->value < rhs.value;
		}

		const string& TokenValue::to_string() const noexcept {
			return this->value;
		}

		// ****************** CharClassTokenValue  *****************

		bool CharClassTokenValue::operator==(const CharClassTokenValue&rhs) const noexcept {

		}

		bool CharClassTokenValue::operator!=(const CharClassTokenValue&rhs) const noexcept {

		}

		bool CharClassTokenValue::operator>(const CharClassTokenValue&rhs) const noexcept {

		}

		bool CharClassTokenValue::operator<(const CharClassTokenValue&rhs) const noexcept {

		}

		const string& CharClassTokenValue::to_string() const noexcept {

		}


		// ****************** Token  *******************************

		bool Token::operator==(const Token &rhs) const noexcept {
			// a faster comparison
			if (this->type != rhs.type) { return false; }
			return *this->value == *rhs.value;
		}

		bool Token::operator!=(const Token &rhs) const noexcept {
			return *this->value != *rhs.value;
		}

		bool Token::operator>(const Token &rhs) const noexcept {
			return *this->value > *rhs.value;
		}

		bool Token::operator<(const Token &rhs) const noexcept {
			return *this->value < *rhs.value;
		}

		string Token::type_string() const noexcept {
			string str(std::to_string((int)this->type));
			return std::move(str);
		}

		string Token::to_string() const noexcept {
			string str("[");
			str += this->type_string();
			str += ", ";
			if (this->value != nullptr) {
				str += this->value->to_string();
			} else {
				str += "_NONE_";
			}
			str += "]";
			return std::move(str);
		}
	}
}