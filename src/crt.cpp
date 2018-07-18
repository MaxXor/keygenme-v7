#include "crt.h"
#include "injection.h"

_wcsncmp vwcsncmp = NULL;
_memset  vmemset  = NULL;
_wcsncpy vwcsncpy = NULL;
_memcpy  vmemcpy  = NULL;
_InitCommonControls vInitCommonControls = NULL;

void CRTInit()
{
	HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

	vwcsncmp = (_wcsncmp)GetProcAddress(ntdll, "wcsncmp");
	vmemset = (_memset)GetProcAddress(ntdll, "memset");
	vwcsncpy = (_wcsncpy)GetProcAddress(ntdll, "wcsncpy");
	vmemcpy = (_memcpy)GetProcAddress(ntdll, "memcpy");

	HMODULE comctl32dll = GetModuleHandleW(L"comctl32.dll");
	if (comctl32dll == NULL)
		comctl32dll = LoadLibraryW(L"comctl32.dll");

	vInitCommonControls = (_InitCommonControls)GetProcAddress(comctl32dll, "InitCommonControls");

	if (!GetDebugPrivileges())
		ExitProcess(0);
}