#pragma once

#include <string>
#include <memory>
#include <map>
#include <utility> 
#include <vector>
#include <queue>
#include <stack>
#include <set>

#ifdef _CORE_DEBUG
#include <iostream>
#endif // _CORE_DEBUG


#include "state.h"
#include "token.h"

#include "../util/noncopyable.h"

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::vector;
using std::queue;
using std::stack;
using std::set;

namespace nativejs {
	namespace core {

		class DFA {
		private:
			State::Ptr start;
			State::Set ends;

		public:
			DFA(State::Ptr& start, State::Set&& ends) :start(start), ends(std::move(ends)) {}
		};


		class NFA : noncopyable {
		private:
			State::Ptr start;
			State::Ptr end;

			// ******** private constrcutors *****************

			NFA() : start(make_shared<State>(false)), end(make_shared<State>(true)) {
			}

			NFA(State::Ptr start, State::Ptr end) : start(start), end(end) {}

			// add a edge to the states graph
			void push_transition(const State::Ptr &start, const State::Ptr &end, const Token &token = Token::epsilon) {
				start->push_transition(token, end);
			}

		public:
			explicit NFA(const Token &token) : start(make_shared<State>(false)), end(make_shared<State>(true)) {
				this->push_transition(this->start, this->end, token);
			}

			NFA(const NFA&& rhs) noexcept : start(rhs.start), end(rhs.end) {}

			NFA& operator=(const NFA& rhs) {
				this->start = rhs.start;
				this->end = rhs.end;
				// this->default_token = rhs.default_token;
				return *this;
			}

			// {0,1} ~nfa
			// {0,} *-closure: (~nfa)++
			// {1,} +-closure: nfa++
			// r = st -> s + t
			// r = s|t -> s | t

			// for alternate, {0,1} -> ~nfa
			NFA operator~() {
				// this->start->push_rule(Token::epsilon, this->end);
				NFA nfa;
				push_transition(nfa.start, nfa.end);
				push_transition(nfa.start, this->start);
				push_transition(this->end, nfa.end);

				return ::std::move(nfa);
			}

			// for +closure, {1,} -> nfa++
			NFA &operator++(int) {
				push_transition(this->end, this->start);

				return *this;
			}

			// for concat
			NFA operator+(const NFA &rhs) {
				NFA nfa(this->start, rhs.end);
				push_transition(this->end, rhs.start);

				return std::move(nfa);
			}

			// for compose
			NFA operator|(const NFA &rhs) {
				NFA nfa;
				push_transition(nfa.start, this->start);
				push_transition(this->end, nfa.end);

				push_transition(nfa.start, rhs.start);
				push_transition(rhs.end, nfa.end);

				return std::move(nfa);
			}

			shared_ptr<State> epsilon_closure(const State::Ptr &_t) const {
				stack<State::Ptr> state_stack;
				auto e_closure = make_shared<State>();

				if (_t->is_simple()) {
					e_closure->push_child(_t);
				} else {
					auto push_child_worker = [&e_closure](const State::Ptr& u) {
						e_closure->push_child(u);
					};
					_t->for_children(push_child_worker);
				}

				state_stack.push(_t);

				auto worker = [&state_stack, &e_closure](const State::Ptr& u) {
					// if `u` is not in e_closure
					if (e_closure->push_child(u)) {
						state_stack.push(u);
					}
				};

				while (!state_stack.empty()) {
					auto &t = state_stack.top();
					state_stack.pop();

					t->for_transition(Token::epsilon, worker);
				}

				return e_closure;
			}


			// move(T, a)
			const shared_ptr<State> move(const State::Ptr &T, const Token &a) const {
				auto closure = make_shared<State>();

				auto worker = [&closure](const State::Ptr& state) {
					closure->push_child(state);
				};

				auto status = T->for_transition(a, worker);
				return closure;
			}

			const set<Token> token_list() {
				/*auto _a = make_shared<Token>(TokenType::LITERAL, "a");
				auto _b = make_shared<Token>(TokenType::LITERAL, "b");*/
				Token _a(TokenType::LITERAL, "a");
				Token _b(TokenType::LITERAL, "b");
				set<Token> _list{ _a, _b };

				return std::move(_list);
			}

			State::Set subset_construct() {
				auto initial_state = epsilon_closure(this->start);
				auto alphabet = this->token_list();
				queue<State::Ptr> unlabelled;

				State::Set dfa_state_history;
				State::Set dfa_states;
				dfa_state_history.insert(initial_state);
				dfa_states.insert(initial_state);
				unlabelled.push(initial_state);

				while (!unlabelled.empty()) {
					auto& T = unlabelled.front();
					unlabelled.pop();

					for (auto &a : alphabet) {
						auto U = epsilon_closure(move(T, a));

						if (dfa_state_history.find(U) == dfa_state_history.end()) {
							dfa_state_history.insert(U);
							unlabelled.push(U);
						}
						// T->push_rule(a, U);
						// Dtran[T, a] = U;
#ifdef _CORE_DEBUG
						std::cout << T->to_string(false) << " -> "
							<< a.to_string() << " -> "
							<< U->to_string(false) << std::endl;
#endif // _CORE_DEBUG
					}
				}

				return std::move(dfa_state_history);
			}
		};
	}
}