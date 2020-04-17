#pragma once

#include <string_view>

namespace lxd {
	// Convert a wide Unicode string to an UTF8 string
	std::string utf8_encode(const std::wstring_view wstr);
	// Convert an UTF8 string to a wide Unicode String
	std::wstring utf8_decode(const std::string_view str);
}