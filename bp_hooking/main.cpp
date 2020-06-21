#include "hook_manager.h"
#include <iostream>

typedef HANDLE	(__stdcall *hk_CreateFileW_t)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE	(__stdcall *hk_FindFirstFileW_t)(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
typedef BOOL	(__stdcall *hk_FindNextFileW_t)(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
typedef BOOL	(__stdcall *hk_FindClose_t)(HANDLE hFindFile);
typedef BOOL	(__stdcall *hk_ReadFile_t)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
typedef DWORD	(__stdcall *hk_SetFilePointer_t)(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
typedef BOOL	(__stdcall *hk_SetFilePointerEx_t)(HANDLE hFile,	LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
typedef DWORD	(__stdcall *hk_GetFileAttributesW_t)(LPCWSTR lpFileName);
typedef BOOL	(__stdcall *hk_GetFileInformationByHandle_t)(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation);
typedef BOOL	(__stdcall *hk_GetFileInformationByHandleEx_t)(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass,	LPVOID lpFileInformation, DWORD dwBufferSize);
typedef DWORD	(__stdcall *hk_GetFileType_t)(HANDLE hFile);
typedef BOOL	(__stdcall *hk_CloseHandle_t)(HANDLE hObject);
typedef FARPROC (__stdcall *hk_GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
typedef BOOL	(__stdcall *hk_GetModuleHandleExW_t)(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE*phModule);

HANDLE	__stdcall hk_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE	__stdcall hk_FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
BOOL	__stdcall hk_FindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
BOOL	__stdcall hk_FindClose(HANDLE hFindFile);
BOOL	__stdcall hk_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
DWORD	__stdcall hk_SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
BOOL	__stdcall hk_SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
DWORD	__stdcall hk_GetFileAttributesW(LPCWSTR lpFileName);
BOOL	__stdcall hk_GetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation);
BOOL	__stdcall hk_GetFileInformationByHandleEx(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize);
DWORD	__stdcall hk_GetFileType(HANDLE hFile);
BOOL	__stdcall hk_CloseHandle(HANDLE hObject);
BOOL	__stdcall hk_GetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE* phModule);
FARPROC	__stdcall hk_GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

long __stdcall handler(PEXCEPTION_POINTERS info);

int main() {

	hook_manager::get().init(handler);
	defer(hook_manager::get().deinit());

	auto& mgr = hook_manager::get();

	HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");

	if (kernel32 == INVALID_HANDLE_VALUE) return -1;

	mgr["GetProcAddress"]->hook(GetProcAddress(kernel32, "GetProcAddress"), hk_GetProcAddress);
	mgr["CreateFileW"]->hook(GetProcAddress(kernel32, "CreateFileW"), hk_CreateFileW);
	mgr["FindFirstFileW"]->hook(GetProcAddress(kernel32, "FindFirstFileW"), hk_FindFirstFileW);
	mgr["FindNextFileW"]->hook(GetProcAddress(kernel32, "FindNextFileW"), hk_FindNextFileW);
	mgr["ReadFile"]->hook(GetProcAddress(kernel32, "ReadFile"), hk_ReadFile);
	mgr["FindClose"]->hook(GetProcAddress(kernel32, "FindClose"), hk_FindClose);
	mgr["SetFilePointer"]->hook(GetProcAddress(kernel32, "SetFilePointer"), hk_SetFilePointer);
	mgr["SetFilePointerEx"]->hook(GetProcAddress(kernel32, "SetFilePointerEx"), hk_SetFilePointerEx);
	mgr["GetFileAttributesW"]->hook(GetProcAddress(kernel32, "GetFileAttributesW"), hk_GetFileAttributesW);
	mgr["GetFileInformationByHandle"]->hook(GetProcAddress(kernel32, "GetFileInformationByHandle"), hk_GetFileInformationByHandle);
	mgr["GetFileInformationByHandleEx"]->hook(GetProcAddress(kernel32, "GetFileInformationByHandleEx"), hk_GetFileInformationByHandleEx);
	mgr["GetFileType"]->hook(GetProcAddress(kernel32, "GetFileType"), hk_GetFileType);
	mgr["CloseHandle"]->hook(GetProcAddress(kernel32, "CloseHandle"), hk_CloseHandle);
	mgr["GetModuleHandleExW"]->hook(GetProcAddress(kernel32, "GetModuleHandleExW"), hk_GetModuleHandleExW);

	WIN32_FIND_DATA data;

	for (int i = 0; i < 2; i++) {
		try {

			std::cout << "=========\n";
			std::cout << "Iteration " << i << std::endl;
			std::cout << "=========\n";

			CloseHandle(nullptr);
			FindFirstFileW(L"test.txt", nullptr);
			FindNextFileW(CreateFileW(L"test.txt", 0, 0, nullptr, 0, 0, nullptr), &data);
			FindClose(nullptr);
			ReadFile(nullptr, nullptr, 0, nullptr, nullptr);
			SetFilePointer(nullptr, 0, nullptr, 0);
			SetFilePointerEx(nullptr, LARGE_INTEGER(), nullptr, 0);
			GetFileAttributesW(L"test.txt");
			GetFileInformationByHandle(nullptr, nullptr);
			GetFileInformationByHandleEx(nullptr, FILE_INFO_BY_HANDLE_CLASS(), nullptr, 0);
			GetFileType(nullptr);
			GetProcAddress(GetModuleHandleA("kernelbase.dll"), "test");
			GetModuleHandleExW(0, L"kernelbase.dll", nullptr);
		}
		catch (std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	return 0;
}

long __stdcall handler(PEXCEPTION_POINTERS info) {

	if (info->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT)
		return EXCEPTION_CONTINUE_SEARCH;

	bp_hook* hk = hook_manager::get_hook(info);
	if(hk == nullptr)
		return EXCEPTION_CONTINUE_EXECUTION;

	info->ContextRecord->Ip = hk->hook_addr<uintptr_t>();

	return EXCEPTION_CONTINUE_EXECUTION;
}


HANDLE __stdcall hk_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile){
	hook_entry("CreateFileW"); 
	std::cout << "hk_CreateFileW" << std::endl;
	return hook_manager::get()["CreateFileW"]->original<hk_CreateFileW_t>()(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE __stdcall hk_FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData){
	hook_entry("FindFirstFileW"); 
	std::cout << "hk_FindFirstFileW" << std::endl;
	return hook_manager::get()["FindFirstFileW"]->original<hk_FindFirstFileW_t>()(lpFileName, lpFindFileData);
}

BOOL __stdcall hk_FindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData){
	hook_entry("FindNextFileW"); 
	std::cout << "hk_FindNextFileW" << std::endl;
	return hook_manager::get()["FindNextFileW"]->original<hk_FindNextFileW_t>()(hFindFile, lpFindFileData);
}

BOOL __stdcall hk_FindClose(HANDLE hFindFile){
	hook_entry("FindClose"); 
	std::cout << "hk_FindClose" << std::endl;
	return hook_manager::get()["FindClose"]->original<hk_FindClose_t>()(hFindFile);
}

BOOL __stdcall hk_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped){
	hook_entry("ReadFile"); 
	std::cout << "hk_ReadFile" << std::endl;
	return hook_manager::get()["ReadFile"]->original<hk_ReadFile_t>()(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

DWORD __stdcall hk_SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod){
	hook_entry("SetFilePointer"); 
	std::cout << "hk_SetFilePointer" << std::endl;
	return hook_manager::get()["SetFilePointer"]->original<hk_SetFilePointer_t>()(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}

BOOL __stdcall hk_SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod){
	hook_entry("SetFilePointerEx"); 
	std::cout << "hk_SetFilePointerEx" << std::endl;
	return hook_manager::get()["SetFilePointerEx"]->original<hk_SetFilePointerEx_t>()(hFile, liDistanceToMove, lpNewFilePointer, dwMoveMethod);
}

DWORD __stdcall hk_GetFileAttributesW(LPCWSTR lpFileName){
	hook_entry("GetFileAttributesW"); 
	std::cout << "hk_GetFileAttributesW" << std::endl;
	return hook_manager::get()["GetFileAttributesW"]->original<hk_GetFileAttributesW_t>()(lpFileName);
}

BOOL __stdcall hk_GetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation){
	hook_entry("GetFileInformationByHandle"); 
	std::cout << "hk_GetFileInformationByHandle" << std::endl;
	return hook_manager::get()["GetFileInformationByHandle"]->original<hk_GetFileInformationByHandle_t>()(hFile, lpFileInformation);
}

BOOL __stdcall hk_GetFileInformationByHandleEx(HANDLE hFile, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, LPVOID lpFileInformation, DWORD dwBufferSize){
	hook_entry("GetFileInformationByHandleEx"); 
	std::cout << "hk_GetFileInformationByHandleEx" << std::endl;
	return hook_manager::get()["GetFileInformationByHandleEx"]->original<hk_GetFileInformationByHandleEx_t>()(hFile, FileInformationClass, lpFileInformation, dwBufferSize);
}

DWORD __stdcall hk_GetFileType(HANDLE hFile){
	hook_entry("GetFileType"); 
	std::cout << "hk_GetFileType" << std::endl;
	return hook_manager::get()["GetFileType"]->original<hk_GetFileType_t>()(hFile);
}

BOOL __stdcall hk_CloseHandle(HANDLE hObject){
	hook_entry("CloseHandle"); 
	std::cout << "hk_CloseHandle" << std::endl;
	return hook_manager::get()["CloseHandle"]->original<hk_CloseHandle_t>()(hObject);
}

FARPROC _stdcall hk_GetProcAddress(HMODULE hModule, LPCSTR lpProcName){
	hook_entry("GetProcAddress"); 
	std::cout << "hk_GetProcAddress -> " << lpProcName << std::endl;
	return hook_manager::get()["GetProcAddress"]->original<hk_GetProcAddress_t>()(hModule, lpProcName);
}

BOOL __stdcall hk_GetModuleHandleExW(DWORD dwFlags, LPCWSTR lpModuleName, HMODULE* phModule){
	hook_entry("GetModuleHandleExW"); 
	std::cout << "hk_GetModuleHandleExW" << std::endl;
	return hook_manager::get()["GetModuleHandleExW"]->original<hk_GetModuleHandleExW_t>()(dwFlags, lpModuleName, phModule);
}











