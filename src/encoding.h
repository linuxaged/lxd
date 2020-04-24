#pragma once

#include "defines.h"
#include <string_view>

namespace lxd {
	// Convert a wide Unicode string to an UTF8 string
	DLL_PUBLIC std::string utf8_encode(const std::wstring_view wstr);
	// Convert an UTF8 string to a wide Unicode String
	DLL_PUBLIC std::wstring utf8_decode(const std::string_view str);
}