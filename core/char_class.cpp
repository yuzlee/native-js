#include "char_class.h"

namespace nativejs {
	namespace core {
		const CharClass CharClass::Common::number = CharClass([](const CharClass::CharType& _t) {
			return _t >= '0' && _t <= '9'; // [0-9]
		});

		const CharClass CharClass::Common::not_number = CharClass([](const CharClass::CharType& _t) {
			return !(_t >= '0' && _t <= '9'); // [^0-9]
		});

		const CharClass CharClass::Common::ascii_uppercase = CharClass([](const CharClass::CharType& _t) {
			return _t >= 'A' && _t <= 'Z'; // [A-Z]
		});

		const CharClass CharClass::Common::ascii_lowercase = CharClass([](const CharClass::CharType& _t) {
			return _t >= 'a' && _t <= 'z'; // [a-z]
		});

		const CharClass CharClass::Common::ascii_letter = CharClass([](const CharClass::CharType& _t) {
			return (_t >= 'a' && _t <= 'z') || (_t >= 'A' && _t <= 'Z'); // [a-zA-Z]
		});

		const CharClass CharClass::Common::not_ascii_letter = CharClass([](const CharClass::CharType& _t) {
			return !((_t >= 'a' && _t <= 'z') || (_t >= 'A' && _t <= 'Z')); // [^a-zA-Z]
		});

		const CharClass CharClass::Common::ascii_char = CharClass([](const CharClass::CharType& _t) {
			return (_t >= 'a' && _t <= 'z') || (_t >= 'A' && _t <= 'Z') || (_t >= '0' && _t <= '9'); // [a-zA-Z0-9]
		});

		const CharClass CharClass::Common::not_ascii_char = CharClass([](const CharClass::CharType& _t) {
			return !((_t >= 'a' && _t <= 'z') || (_t >= 'A' && _t <= 'Z') || (_t >= '0' && _t <= '9')); // [^a-zA-Z0-9]
		});

		const CharClass CharClass::Common::blank = CharClass([](const CharClass::CharType& _t) {
			return _t == ' '; // blank
		});

		const CharClass CharClass::Common::not_blank = CharClass([](const CharClass::CharType& _t) {
			return _t != ' '; // blank
		});

		const CharClass CharClass::Common::underline = CharClass([](const CharClass::CharType& _t) {
			return _t == '_'; // [_]
		});

		const CharClass CharClass::Common::new_line = CharClass([](const CharClass::CharType& _t) {
			return _t == '\n' || _t == '\r'; // \r\n
		});

		const CharClass CharClass::Common::not_new_line = CharClass([](const CharClass::CharType& _t) {
			return !(_t == '\n' || _t == '\r'); // \r\n
		});
	}
}