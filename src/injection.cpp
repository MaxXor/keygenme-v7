#include "injection.h"

void func(INJECTED_DATA *data)
{
	typedef int (WINAPI *_MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);

	_MessageBoxW vMessageBoxW = (_MessageBoxW)data->MessageBoxWPtr;

	vMessageBoxW(data->Hwnd, data->Text, data->Title, MB_ICONWARNING + MB_OK);
}

void ende(void) { return; }

bool GetDebugPrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tPrivileges;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &tPrivileges.Privileges[0].Luid);
		tPrivileges.PrivilegeCount = 1;
		tPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		return AdjustTokenPrivileges(hToken, 0, &tPrivileges, sizeof(tPrivileges), NULL, NULL);
	}
	return false;
}

void injectcode(INJECTED_DATA *param)
{
	HANDLE hThread;
	LPVOID paramMem, functionMem;
	DWORD length = (DWORD)ende - (DWORD)func;

	HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());

	functionMem = VirtualAllocEx(hProc, NULL, length, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	paramMem = VirtualAllocEx(hProc, NULL, sizeof(INJECTED_DATA), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	WriteProcessMemory(hProc, functionMem, (LPVOID)func, length, NULL);
	WriteProcessMemory(hProc, paramMem, param, sizeof(INJECTED_DATA), NULL);

	hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)functionMem, paramMem, 0, NULL);

	CloseHandle(hProc);

	Sleep(200);
}