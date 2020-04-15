#include "fileio.h"
#include <Windows.h>
#include <fileapi.h>

namespace lxd {
	
	std::string ReadIntoString(std::string_view path) {
		return std::string();
	}
	std::string ReadIntoString(std::wstring_view path) {
		// open the file
		CREATEFILE2_EXTENDED_PARAMETERS param = {};
		param.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		param.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
		param.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
		param.dwSecurityQosFlags = SECURITY_ANONYMOUS;
		auto handle = CreateFile2(path.data(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &param);
		if(INVALID_HANDLE_VALUE == handle) {
			CloseHandle(handle);
			return {};
		}
		// get file size
		LARGE_INTEGER size;
		if(!GetFileSizeEx(handle, &size)) {
			CloseHandle(handle);
			return {};
		}
		// read
		std::string buffer;
		buffer.resize(size.QuadPart);
		if(!ReadFile(handle, buffer.data(), buffer.size(), nullptr, nullptr)) {
			CloseHandle(handle);
			return {};
		}

		CloseHandle(handle);
		return buffer;
	}
}