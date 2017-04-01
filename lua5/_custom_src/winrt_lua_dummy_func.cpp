// Dummy implementation of legacy CRT functions / Win32 APIs for Windows Store App.

#include "winrt_lua_dummy_func.h"
#include <errno.h>

#ifdef DEFINES_MY_LUA_DUMMY_FUNCS_FOR_WINRT

// http://msdn.microsoft.com/library/277bwbdz.aspx
int system(const char* command)
{
	errno = ENOENT;
	return 0;
}

// http://msdn.microsoft.com/library/96ayss4b.aspx
FILE* _popen(const char* command, const char* mode)
{
	return nullptr;
}

// http://msdn.microsoft.com/library/25xdhsd2.aspx
int _pclose(FILE* stream)
{
	return -1;
}

// http://msdn.microsoft.com/library/tehxacec.aspx
char* getenv(const char* varname)
{
	return nullptr;
}

#ifndef LUA_ANSI
// http://msdn.microsoft.com/library/windows/desktop/ms683197.aspx
DWORD GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
#if 0
	::SetLastError(ERROR_NOT_SUPPORTED);
	return 0; // Failed to boot Lua...
#else
	const char* dummyFileName = "_:\\WinRTLuaDummy\\WinRTLuaDummy.exe";
	strcpy_s(lpFilename, nSize, dummyFileName); // Dummy path.
	return static_cast<DWORD>(strlen(dummyFileName));
#endif
}
#endif

// http://msdn.microsoft.com/library/windows/desktop/ms684179.aspx
HMODULE WINAPI LoadLibraryExA(_In_ LPCSTR lpFileName, _Reserved_ HANDLE hFile, _In_ DWORD dwFlags)
{
#if 0
	::SetLastError(ERROR_NOT_SUPPORTED);
	return NULL;
#else
	WCHAR utf16FileName[1024] = {};
	// ANSI multibyte characters are not available on Windows Store App. However, ANSI ONLY characters or UTF-8 are still available.
	int retVal = ::MultiByteToWideChar(CP_UTF8, 0, lpFileName, static_cast<int>(strlen(lpFileName)), utf16FileName, sizeof(utf16FileName) / sizeof(*utf16FileName));
	return (retVal > 0) ? ::LoadPackagedLibrary(utf16FileName, 0) : nullptr;
#endif
}

#endif
