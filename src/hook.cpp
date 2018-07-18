#include "hook.h"

CreateWindowExWPtr OldCreateWindowExW = NULL;
CharUpperWPtr OldCharUpperW = NULL;

DWORD __stdcall PrepareHook(DWORD func, DWORD hook)
{
	DWORD oldProtection;
	DWORD jumpWay;

	LPVOID hookMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 10);
	VirtualProtect(hookMem, 10, PAGE_EXECUTE_READWRITE, &oldProtection);
	VirtualProtect((LPVOID)func, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

	vmemcpy((CHAR*)hookMem, (CHAR*)func, 5);
	vmemset((CHAR*)func, 0xE9, 1);
	jumpWay = hook - (func + 5);
	vmemcpy((CHAR*)func + 1, &jumpWay, 4);
	vmemset((CHAR*)hookMem + 5, 0xE9, 1);
	jumpWay = (func + 5) - ((DWORD)hookMem + 10);
	vmemcpy((CHAR*)hookMem + 6, &jumpWay, 4);
	return (DWORD)hookMem;
}