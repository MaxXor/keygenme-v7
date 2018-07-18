#pragma once

#include <windows.h>

struct INJECTED_DATA
{
	DWORD MessageBoxWPtr;
	HWND Hwnd;
	WCHAR Title[11];
	WCHAR Text[69];
};

bool GetDebugPrivileges();
void injectcode(INJECTED_DATA*);