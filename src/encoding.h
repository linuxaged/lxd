#pragma once

#include <string_view>

namespace lxd {
	std::wstring UTF8Decode(std::string_view string);
	std::string UTF8Encode(std::wstring_view wstring);
}