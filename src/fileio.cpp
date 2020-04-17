#include "fileio.h"
#include <Windows.h>
#include <fileapi.h>

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

	bool openModeCanCreate(OpenMode openMode) {
		// WriteOnly can create, but only when ExistingOnly isn't specified.
		// ReadOnly by itself never creates.
		return (openMode & OpenMode::WriteOnly) && !(openMode & OpenMode::ExistingOnly);
	}

	HANDLE OpenFile(std::wstring_view path, OpenMode openMode) {
		// All files are opened in share mode (both read and write).
		DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		int accessRights = 0;
		if(openMode & OpenMode::ReadOnly)
			accessRights |= GENERIC_READ;
		if(openMode & OpenMode::WriteOnly)
			accessRights |= GENERIC_WRITE;
		// WriteOnly can create files, ReadOnly cannot.
		DWORD creationDisp = (openMode & OpenMode::NewOnly)
			? CREATE_NEW
			: openModeCanCreate(openMode)
			? OPEN_ALWAYS
			: OPEN_EXISTING;

		CREATEFILE2_EXTENDED_PARAMETERS param = {};
		param.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		param.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
		param.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
		param.dwSecurityQosFlags = SECURITY_ANONYMOUS;
		return CreateFile2(path.data(), accessRights, shareMode, creationDisp, &param);
	}

	bool fwrite(std::wstring_view path, char const* buffer, size_t bufferSize) {
		// open the file
		auto handle = OpenFile(path, OpenMode::WriteOnly);
		if(INVALID_HANDLE_VALUE == handle) {
			return false;
		}
		// get file size
		LARGE_INTEGER size;
		if(!GetFileSizeEx(handle, &size)) {
			CloseHandle(handle);
			return false;
		}
		// write
		if(!WriteFile(handle, buffer, bufferSize, nullptr, nullptr)) {
			CloseHandle(handle);
			return false;
		}
		CloseHandle(handle);
		return true;
	}

	std::string fread(std::string_view path) {
		return std::string();
	}
	std::string fread(std::wstring_view path) {
		// open the file
		auto handle = OpenFile(path, OpenMode::ReadOnly);
		if(INVALID_HANDLE_VALUE == handle) {
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