#pragma once

#include <cstdio>
#include <iostream>

using real_t = double;

#define rep(x, y) for (auto x = decltype(y){0}; x != y; ++x)
#define repa(x, y, z) for (auto x = decltype(z){y}; x != z; ++x)
#define repd(x, y, z) for (auto x = decltype(z){y}; x != z; --x)

template<typename T>
inline void updateMax(T &value, const T &max) {
	if (value < max) value = max;
}

__declspec(noreturn) inline void error_exit(const char *msg) {
	printf("%s", msg);
	exit(0);
}