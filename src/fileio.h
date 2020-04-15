#pragma once

#include <string_view>

namespace lxd {
	std::string ReadIntoString(std::string_view path);
	std::string ReadIntoString(std::wstring_view path);
}