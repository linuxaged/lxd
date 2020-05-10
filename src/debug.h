#pragma once

#include <fmt/format.h>
#include <debugapi.h>

namespace lxd {
	template <typename... Args>
	static void print(char const* format_str, Args&&... args) {
		std::string output = fmt::format(format_str, std::forward<Args>(args)...);
		OutputDebugStringA(output.c_str());
	}

	template <typename... Args>
	static void print(wchar_t const* format_str, Args&&... args) {
		std::wstring output = fmt::format(format_str, std::forward<Args>(args)...);
		OutputDebugStringW(output.c_str());
	}
}