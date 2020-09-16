#pragma once

#include "defines.h"
#include <string_view>

namespace lxd {
	// Convert a wide Unicode string to an UTF8 string
	DLL_PUBLIC std::string utf8_encode(std::wstring_view wstr);
	// Convert an UTF8 string to a wide Unicode String
	DLL_PUBLIC std::wstring utf8_decode(std::string_view str);

	DLL_PUBLIC size_t uri_encode(const char* src, const size_t len, char* dst);
	DLL_PUBLIC size_t uri_decode(const char* src, const size_t len, char* dst);

	DLL_PUBLIC std::string hex_encode(const unsigned char* content, size_t size);
}