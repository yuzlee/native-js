#pragma once

#include <memory>

using std::shared_ptr;
using std::binary_function;

template <class T>
struct PtrLess : public binary_function<shared_ptr<T>, shared_ptr<T>, bool> {
	bool operator()(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) const noexcept {
		return *lhs < *rhs;
	}
};

template <class T>
struct Less : public binary_function<T, T, bool> {
	bool operator()(const T& lhs, const T& rhs) const noexcept {
		return lhs < rhs;
	}
};