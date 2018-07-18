#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN

__forceinline void JUNK_CODE()
{
	__asm{push eax}
	__asm{xor eax, eax}
	__asm{setpo al}
	__asm{push edx}
	__asm{xor edx, eax}
	__asm{sal edx, 2}
	__asm{xchg eax, edx}
	__asm{pop edx}
	__asm{or eax, ecx}
	__asm{pop eax}
}

#define _sizeof(array) (sizeof(array)/sizeof(array[0]))

// STRING ENCRYPT0R
//
//#include <windows.h>
//#include <stdio.h>
//
//#define _sizeof(array) (sizeof(array)/sizeof(array[0]))
//
//int main()
//{
//	int i;
//
//	WCHAR input[] = L"YOUR-INPUT";
//	for (int temp = 0; temp < _sizeof(input) - 1; temp++)
//		input[temp] = input[temp] ^ (0x12814 + temp) % 255;
//
//	printf("WCHAR temp[] = { ");
//	for (i = 0; i < _sizeof(input); i++)
//	{
//		printf("0x%02X", input[i]);
//		if (i + 1 < _sizeof(input))
//			printf(", ");
//		else
//			printf(" ");
//	}
//
//	printf("};");
//	return 0;
//}