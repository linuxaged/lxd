#include "fileio.h"
#include <Windows.h>
#include <fileapi.h>
#include <fmt/format.h>

namespace lxd {
	bool openModeCanCreate(OpenMode openMode) {
		// WriteOnly can create, but only when ExistingOnly isn't specified.
		// ReadOnly by itself never creates.
		return (openMode & OpenMode::WriteOnly) && !(openMode & OpenMode::ExistingOnly);
	}

	void* OpenFile(std::wstring_view path, OpenMode openMode) {
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

	bool CloseFile(void* handle) {
		return CloseHandle(handle);
	}

	bool WriteFile(std::wstring_view path, char const* buffer, size_t bufferSize) {
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
		if(!::WriteFile(handle, buffer, bufferSize, nullptr, nullptr)) {
			CloseHandle(handle);
			return false;
		}
		CloseFile(handle);
		return true;
	}

	std::string ReadFile(std::wstring_view path) {
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
		if(!::ReadFile(handle, buffer.data(), buffer.size(), nullptr, nullptr)) {
			CloseHandle(handle);
			return {};
		}

		CloseFile(handle);
		return buffer;
	}

	bool CreateDir(std::wstring_view path) {
		return ::CreateDirectoryW(path.data(), nullptr);
	}

	int DeleteDir(std::wstring_view path, bool bDeleteSubdirectories) {
		bool				bSubdirectory = false; // Flag, indicating whether
												   // subdirectories have been found
		HANDLE				hFile;                 // Handle to directory
		std::wstring		strFilePath;           // Filepath
		std::wstring		strPattern;            // Pattern
		WIN32_FIND_DATAW	FileInformation;       // File information


		strPattern = fmt::format(L"{0}{1}", path, L"\\*.*");
		hFile = ::FindFirstFileW(strPattern.c_str(), &FileInformation);
		if(hFile != INVALID_HANDLE_VALUE) {
			do {
				if(FileInformation.cFileName[0] != '.') {
					strFilePath.erase();
					strFilePath = fmt::format(L"{0}{1}{2}", path, L"\\", FileInformation.cFileName);

					if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						if(bDeleteSubdirectories) {
							// Delete subdirectory
							int iRC = DeleteDir(strFilePath, bDeleteSubdirectories);
							if(iRC)
								return iRC;
						} else
							bSubdirectory = true;
					} else {
						// Set file attributes
						if(::SetFileAttributesW(strFilePath.c_str(),
											   FILE_ATTRIBUTE_NORMAL) == FALSE)
							return ::GetLastError();

						// Delete file
						if(::DeleteFileW(strFilePath.c_str()) == FALSE)
							return ::GetLastError();
					}
				}
			} while(::FindNextFileW(hFile, &FileInformation) == TRUE);

			// Close handle
			::FindClose(hFile);

			DWORD dwError = ::GetLastError();
			if(dwError != ERROR_NO_MORE_FILES)
				return dwError;
			else {
				if(!bSubdirectory) {
					// Set directory attributes
					if(::SetFileAttributesW(path.data(),
										   FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();

					// Delete directory
					if(::RemoveDirectoryW(path.data()) == FALSE)
						return ::GetLastError();
				}
			}
		}

		return 0;
	}

	bool DirExists(std::wstring_view path) {
		DWORD attrib = GetFileAttributesW(path.data());
		return (attrib != INVALID_FILE_ATTRIBUTES) && (attrib & FILE_ATTRIBUTE_DIRECTORY);
	}
}