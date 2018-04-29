#include "../core/state.h"

#include <iostream>
#include <string>
#include <set>

using namespace std;
using namespace nativejs::core;

void ifeq(string &&msg, const State::Ptr &lhs, const State::Ptr &rhs) {
    cout << msg << "\t" << (!(*lhs < *rhs) && !(*rhs < *lhs)) << lhs << rhs << endl;
}

int main() {
    cout << "construct a" << endl;
    auto a = make_shared<State>();
    cout << "construct b" << endl;
    auto b = make_shared<State>();
    cout << "construct c" << endl;
    auto c = make_shared<State>();

    cout << "construct d" << endl;
    auto d = make_shared<State>();
    d->push_child(a);
    d->push_child(b);
    d->push_child(c);

    cout << "construct e" << endl;
    auto e = make_shared<State>();
    e->push_child(a);
	e->push_child(b);
	e->push_child(c);
    e->push_child(b);

    ifeq("a ?= a", a, a);
    ifeq("b ?= c", b, c);
    ifeq("c ?= a", c, a);
    ifeq("a ?= e", a, d);
    ifeq("e ?= g", d, e);

    return 0;
}
