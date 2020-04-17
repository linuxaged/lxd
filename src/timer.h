#pragma once

#include <cstdint>

namespace lxd {
	void sleep(int milliseconds);
	uint64_t nanosecond();
	float millisecond();
	double second();
}