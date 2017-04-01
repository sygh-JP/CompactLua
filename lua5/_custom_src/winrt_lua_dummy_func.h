// Dummy declaration of legacy CRT functions / Win32 APIs for Windows Store App.
// Specify this header as /FI option argument to "loadlib.c", "loslib.c" and "liolib.c".

#pragma once


#ifdef _WIN32
#include <windows.h> // To include <winapifamily.h> indirectly.
#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define DEFINES_MY_LUA_DUMMY_FUNCS_FOR_WINRT
#endif
#endif
#endif


#ifdef DEFINES_MY_LUA_DUMMY_FUNCS_FOR_WINRT

#ifdef ENABLES_LUA_ANSI_MBENC_SUPPORT
#error ANSI multibyte characters are not available on Windows Store App.
#endif

// LUA_ANSI was a previously-used symbol, but seems to have been removed.
#ifndef LUA_ANSI
#pragma message("WARNING: Some Lua platform-dependent extension APIs will not work properly.")
#endif

#include <stdio.h>

// Non-static C function is external linkage.
// To implement following dummy functions internally for static library available in both Desktop App and Store App,
// We have to use other symbols different from those in Desktop libraries.

#ifdef __cplusplus
extern "C"
{
#endif

	int MyWinRTCrtDummy_system(const char* command);
	FILE* MyWinRTCrtDummy_popen(const char* command, const char* mode);
	int MyWinRTCrtDummy_pclose(FILE* stream);
	char* MyWinRTCrtDummy_getenv(const char* varname);
#ifndef LUA_ANSI
	DWORD MyWinRTWin32Dummy_GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
#endif
	HMODULE WINAPI MyWinRTWin32Dummy_LoadLibraryExA(_In_ LPCSTR lpFileName, _Reserved_ HANDLE hFile, _In_ DWORD dwFlags);

#ifdef __cplusplus
}
#endif

#define system MyWinRTCrtDummy_system
#define _popen MyWinRTCrtDummy_popen
#define _pclose MyWinRTCrtDummy_pclose
#define getenv MyWinRTCrtDummy_getenv
#ifndef LUA_ANSI
#define GetModuleFileNameA MyWinRTWin32Dummy_GetModuleFileNameA
#endif
#define LoadLibraryExA MyWinRTWin32Dummy_LoadLibraryExA

#endif
