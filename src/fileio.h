#pragma once

#include <string_view>

namespace lxd {
	bool fwrite(std::wstring_view path, char const* buffer, size_t bufferSize);
	std::string fread(std::string_view path);
	std::string fread(std::wstring_view path);
}