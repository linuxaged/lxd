#pragma once

#include "defines.h"
#include <string_view>

namespace lxd {
	enum OpenMode : short {
		NotOpen = 0x0000,
		ReadOnly = 0x0001,
		WriteOnly = 0x0002,
		ReadWrite = ReadOnly | WriteOnly,
		Append = 0x0004,
		Truncate = 0x0008,
		Text = 0x0010,
		Unbuffered = 0x0020,
		NewOnly = 0x0040,
		ExistingOnly = 0x0080
	};
	DLL_PUBLIC void* OpenFile(std::wstring_view path, OpenMode mode);
	DLL_PUBLIC bool CloseFile(void* handle);
	DLL_PUBLIC bool WriteFile(std::wstring_view path, char const* buffer, size_t bufferSize);
	DLL_PUBLIC std::string ReadFile(std::wstring_view path);

	DLL_PUBLIC bool CreateDir(std::wstring_view path);
	DLL_PUBLIC int DeleteDir(std::wstring_view path, bool bDeleteSubdirectories = true);
	DLL_PUBLIC bool DirExists(std::wstring_view path);
}