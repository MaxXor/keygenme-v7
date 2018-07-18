#pragma once

#include <windows.h>

void CRTInit();

typedef int    (*_wcsncmp)  (const wchar_t *str1, const wchar_t *str2, size_t count);
typedef void*  (*_memset)   (void *ptr, int value, size_t num);
typedef wchar_t* (*_wcsncpy) (wchar_t *str1, const wchar_t *str2, size_t count);
typedef void*  (*_memcpy)   (void *dest, const void *src, size_t count);
typedef void   (*_InitCommonControls) (void);

extern _wcsncmp vwcsncmp;
extern _memset  vmemset;
extern _wcsncpy vwcsncpy;
extern _memcpy  vmemcpy;
extern _InitCommonControls vInitCommonControls;