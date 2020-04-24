#pragma once

#include "defines.h"
#include <cstdint>
#include <string>

namespace lxd {
	DLL_PUBLIC void sleep(int milliseconds);
	DLL_PUBLIC uint64_t nanosecond();
	DLL_PUBLIC float millisecond();
	DLL_PUBLIC double second();
	DLL_PUBLIC const std::string date();
}