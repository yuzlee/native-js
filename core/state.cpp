//
// Created by Rolrence on 4/6/2018.
//
#include "state.h"

#if _CORE_DEBUG
#include <iostream>
#endif // _CORE_DEBUG

namespace nativejs {
	namespace core {
		// ******************** State ***************************

		const string State::id() const noexcept {
			string _id = std::to_string(this->state_id);
			if (this->children != nullptr) {
				_id += '(';
				for (auto& _c : *this->children_id) {
					_id += std::to_string(_c);
					_id += ',';
				}
				_id += "\b)";
			}
			return std::move(_id);
		}

		bool State::is_simple() const noexcept {
			return this->children == nullptr;
		}

		void State::for_children(const function<void(const State::Ptr&)>& func) {
			if (this->children != nullptr) {
				for (auto& child : *this->children) {
					func(child);
				}
			}
		}

		// return true if pushed
		bool State::push_child(const State::Ptr &state) {
			if (this->children == nullptr) {
				this->children = make_shared<State::Set>();
				this->children_id = make_shared<set<size_t>>();
			}
			auto iter = this->children->find(state);
			// if `state` is not in this.children
			if (iter == this->children->end()) {
				this->children->insert(state);
				this->children_id->insert(state->state_id);
				this->children_id_string += std::to_string(state->state_id) + ",";
				// merge all of the transitions of children
				for (auto& _tran : state->transitions) {
					// init
					this->transitions.insert(std::pair<Token, State::Set>(_tran.first, State::Set()));
					this->transitions[_tran.first].insert(_tran.second.begin(), _tran.second.end());
				}
				return true;
			}
			return false;
		}

		void State::push_transition(const Token &token, const State::Ptr &next) {
			// init
			this->transitions.insert(std::pair<Token, State::Set>(token, State::Set()));

			this->transitions[token].insert(next);
			this->is_accept = false;
		}

		void State::all_transitions(const function<void(const Token&, const State::Set&)>& func) const {
			for (auto& _tran : this->transitions) {
				func(_tran.first, _tran.second);
			}
		}

		bool State::for_transition(const Token& token, const function<void(const State::Ptr&)>& func) {
			auto iter = this->transitions.find(token);
			if (iter != this->transitions.end()) {
				for (auto& state : iter->second) {
					func(state);
				}
				return true;
			}
			return false;
		}

		// ******************* operators ***********************

		bool State::operator==(const State &rhs) const noexcept {
#if _CORE_DEBUG
			std::cout << this->id() << " | " << rhs.id() << std::endl;

			if (this->children != nullptr) {
				for (auto& c : *this->children) {
					std::cout << *c << std::endl;
				}
			}
#endif // _CORE_DEBUG
			if (this->children == nullptr) {
				return this->state_id == rhs.state_id;
			}
			for (auto iter1 = this->children_id->begin(), iter2 = rhs.children_id->begin();
				iter1 != this->children_id->end() && iter2 != rhs.children_id->end();
				iter1++, iter2++) {
				if (*iter1 != *iter2) { return false; }
			}
			return true;
		}

		bool State::operator!=(const State &rhs) const noexcept {
			if (this->children == nullptr) {
				return this->state_id != rhs.state_id;
			}
			for (auto iter1 = this->children_id->begin(), iter2 = rhs.children_id->begin();
				iter1 != this->children_id->end() && iter2 != rhs.children_id->end();
				iter1++, iter2++) {
				if (*iter1 == *iter2) { return false; }
			}
			return true;
		}

		bool State::operator>(const State &rhs) const noexcept {
			if (this->is_simple()) { return this->state_id > rhs.state_id; }
			return this->children_id_string > rhs.children_id_string;
		}

		bool State::operator<(const State &rhs) const noexcept {
			if (this->is_simple()) { return this->state_id < rhs.state_id; }
			return this->children_id_string < rhs.children_id_string;
		}

		// ******************* standard IO *********************

		const string State::to_string(bool verbose) const {
			string str = "[";
			str += this->id();
			str += ", ";
			str += (this->is_accept ? "yes]" : "no]");
			// print children
			if (verbose) {
				str += " => { \n";
				for (auto& tran : this->transitions) {
					str += '\t';
					str += tran.first.to_string();
					str += " -> (";
					for (auto& s : tran.second) {
						str += s->to_string(false);
						str += ", ";
					}
					str += "\b\b)\n";
				}
				str += "\b }";
			}

			return str;
		}

		ostream &operator<<(ostream &os, const State &s) {
			return os << s.to_string();
		}

		ostream &operator<<(ostream &os, const State *s) {
			return os << s->to_string();
		}
	}
}
