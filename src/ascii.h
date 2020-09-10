#pragma once

#include "defines.h"
#include <string_view>
#include <algorithm>

namespace lxd {
	namespace ascii_internal {
		// Declaration for an array of bitfields holding character information.
		DLL_PUBLIC extern const unsigned char kPropertyBits[256];
		// Declaration for the array of characters to lower-case characters.
		DLL_PUBLIC extern const char kToLower[256];
	}

	// ascii_isspace()
	//
	// Determines whether the given character is a whitespace character (space,
	// tab, vertical tab, formfeed, linefeed, or carriage return).
	inline bool ascii_isspace(unsigned char c) {
		return (ascii_internal::kPropertyBits[c] & 0x08) != 0;
	}

	// ascii_tolower()
	//
	// Returns an ASCII character, converting to lowercase if uppercase is
	// passed. Note that character values > 127 are simply returned.
	inline char ascii_tolower(unsigned char c) {
		return ascii_internal::kToLower[c];
	}

	// Returns absl::string_view with whitespace stripped from the beginning of the
	// given string_view.
	inline std::string_view StripLeadingAsciiWhitespace(
		std::string_view str) {
		auto it = std::find_if_not(str.begin(), str.end(), lxd::ascii_isspace);
		return str.substr(it - str.begin());
	}

	// Returns absl::string_view with whitespace stripped from the end of the given
	// string_view.
	inline std::string_view StripTrailingAsciiWhitespace(
		std::string_view str) {
		auto it = std::find_if_not(str.rbegin(), str.rend(), lxd::ascii_isspace);
		return str.substr(0, str.rend() - it);
	}

	// Returns absl::string_view with whitespace stripped from both ends of the
	// given string_view.
	inline std::string_view StripAsciiWhitespace(
		std::string_view str) {
		return StripTrailingAsciiWhitespace(StripLeadingAsciiWhitespace(str));
	}

	// EqualsIgnoreCase()
	//
	// Returns whether given ASCII strings `piece1` and `piece2` are equal, ignoring
	// case in the comparison.
	bool EqualsIgnoreCase(std::string_view piece1, std::string_view piece2);
}