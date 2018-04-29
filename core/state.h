#pragma once

#include <ostream>
#include <string>
#include <memory>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <functional>

#include "token.h"
#include "../util/noncopyable.h"
#include "../util/comparison.h"

using std::shared_ptr;
using std::make_shared;
using std::set;
using std::ostream;
using std::string;
using std::map;
using std::function;

namespace nativejs {
	namespace core {
		static size_t global_id = 0;

		static size_t get_global_id() {
			return global_id++;
		}

		class State : noncopyable {
		public:
			typedef shared_ptr<State> Ptr;
			typedef PtrLess<State> Less;
			typedef set<State::Ptr, State::Less> Set;

		private:
			// the id for this state
			size_t state_id = get_global_id();
			// a serialized id
			shared_ptr<set<size_t>> children_id;
			string children_id_string;
			// if this state could be accepted
			bool is_accept;

			shared_ptr<State::Set> children = nullptr;

			map<Token, State::Set> transitions;

		public:
			bool is_visited = false;

			explicit State(bool is_accept = false) : is_accept(is_accept) {}

			const string id() const noexcept;

			bool is_simple() const noexcept;

			void for_children(const function<void(const State::Ptr&)>& func);

			bool push_child(const State::Ptr &state);

			void push_transition(const Token &token, const State::Ptr &next);

			void all_transitions(const function<void(const Token&, const State::Set&)>& func) const;

			bool for_transition(const Token& token, const function<void(const State::Ptr&)>& func);

			// ******************* operators ***********************

			// operator `==` and `!=` is used for comparison
			bool operator==(const State &rhs) const noexcept;

			bool operator!=(const State &rhs) const noexcept;

			// operator `<` and `>` is just used for order
			bool operator>(const State &rhs) const noexcept;

			bool operator<(const State &rhs) const noexcept;


			// ******************* standard IO *********************

			const string to_string(bool verbose = true) const;

			friend ostream &operator<<(ostream &os, const State &s);

			friend ostream &operator<<(ostream &os, const State *s);
		};
	}
}