#pragma once

#include <string_view>

namespace lxd {
	bool WriteIntoFile(std::wstring_view path, char const* buffer, size_t bufferSize);
	std::string ReadIntoString(std::string_view path);
	std::string ReadIntoString(std::wstring_view path);
}