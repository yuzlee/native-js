#pragma once

#include <functional>

using std::function;

namespace nativejs {
	namespace core {
		class CharClass {
		public:
			typedef wchar_t CharType;
			typedef function<bool(const CharType&)> CharComparer;

		private:
			const CharComparer comparer;

		public:
			class Common {
			public:
				static const CharClass number; // [0-9]   \d
				static const CharClass not_number; // [^0-9]  \D

				static const CharClass ascii_uppercase; // [A-Z]
				static const CharClass ascii_lowercase; // [a-z]
				static const CharClass ascii_letter; // [a-zA-Z]
				static const CharClass not_ascii_letter; // [^a-zA-Z]

				static const CharClass ascii_char; // [a-zA-Z0-9] \w
				static const CharClass not_ascii_char; // [^a-zA-Z0-9] \W

				static const CharClass underline; // [_]
				static const CharClass blank; // \s
				static const CharClass not_blank; // \S

				static const CharClass new_line; // \n  \r
				static const CharClass not_new_line; // \n  \r
			};

			// explicit CharClass(const CharComparer comparer) :comparer(comparer) {}

			explicit CharClass(const CharComparer& comparer) :comparer(comparer) {}

			bool is(const CharType& c) const noexcept {
				return this->comparer(c);
			}
		};
	}
}