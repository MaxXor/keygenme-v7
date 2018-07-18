#pragma once

#include <windows.h>
#include "crt.h"

bool GetDebugPrivileges();
// stdcall to clear stack
DWORD __stdcall PrepareHook(DWORD, DWORD);

typedef HWND (WINAPI *CreateWindowExWPtr) (DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

typedef LPWSTR (WINAPI *CharUpperWPtr) (LPWSTR lpsz);

extern CreateWindowExWPtr OldCreateWindowExW;
extern CharUpperWPtr OldCharUpperW;