#include <windows.h>
#include <windowsx.h>
#include "program.h"
#include "crt.h"
#include "xor.h"
#include "injection.h"
#include "hook.h"
#include "resource.h"

#define SERIAL_TIME 0x70000

HWND Hwnd;
HWND TextboxUser;
HWND TextboxSerial;
HWND ButtonRegister;
HWND ButtonAbout;
HWND ButtonExit;

bool EnableCharUpperWHook = false;

void Success()
{
	WCHAR windowTitle[] = { 0x7E, 0x53, 0x4E, 0x5F, 0x5C, 0x54, 0x76, 0x59, 0x1D, 0x68, 0x08, 0x60, 0x6C, 0x62, 0x05, 0x31, 0x29, 0x2A, 0x00 }; // KeygenMe V7 - Full
	WCHAR buttonText[] = { 0x6D, 0x25, 0x26, 0x2B, 0x30, 0x30, 0x20, 0x34, 0x22, 0x2C, 0x00 }; // Registered
	WCHAR messageText[] = { 0x70, 0x57, 0x56, 0x5E, 0x1B, 0x56, 0x52, 0x5C, 0x13, 0x60, 0x2F, 0x2D, 0x34, 0x64, 0x32, 0x34, 0x2E, 0x3C, 0x2C, 0x6A, 0x2A, 0x6C, 0x26, 0x2B, 0x36, 0x37,
							0x34, 0x3C, 0x72, 0x00 }; // Good job, now write a keygen!
	WCHAR messageTitle[] = { 0x6E, 0x4B, 0x5C, 0x23, 0x24, 0x31, 0x30, 0x65, 0x00 }; // Success
	decrypt(windowTitle, 0x12014, _sizeof(windowTitle) - 1);
	decrypt(buttonText, 0x42714, _sizeof(buttonText) - 1);
	decrypt(messageText, 0x32014, _sizeof(messageText) - 1);
	decrypt(messageTitle, 0x12814, _sizeof(messageTitle) - 1);

	SetWindowTextW(Hwnd, windowTitle);
	SetDlgItemTextW(Hwnd, IDC_BUTTONREGIS, buttonText);
	EnableWindow(TextboxUser, false);
	EnableWindow(TextboxSerial, false);
	EnableWindow(ButtonRegister, false);
	MessageBoxW(Hwnd, messageText, messageTitle, MB_OK + MB_ICONINFORMATION);
}

LPWSTR WINAPI CharUpperWHook(LPWSTR lpString)
{
	if (EnableCharUpperWHook)
	{
		for (int i = 0; lpString[i] != '\0'; i++)
		{
			WCHAR temp = lpString[i];
			switch (temp)
			{
			case '1':
				lpString[i] = (WCHAR)((int)temp + 6); // 7
				break;
			case '2':
				lpString[i] = (WCHAR)((int)temp + 2); // 4
				break;
			case '0':
				lpString[i] = (WCHAR)((int)temp + 5); // 5
				break;
			case '7':
				lpString[i] = (WCHAR)((int)temp - 4); // 3
				break;
			case '3':
				lpString[i] = (WCHAR)((int)temp - 2); // 1
				break;
			case '9':
				lpString[i] = (WCHAR)((int)temp - 3); // 6
				break;
			case '4':
				lpString[i] = (WCHAR)((int)temp - 4); // 0
				break;
			case '5':
				lpString[i] = (WCHAR)((int)temp + 4);; // 9
				break;
			case '8':
				lpString[i] = (WCHAR)((int)temp - 6);; // 2
				break;
			case '6':
				lpString[i] = (WCHAR)((int)temp + 2);; // 8
				break;
			}
		}
	}
	return OldCharUpperW(lpString);
}

// TEST SERIAL COMBINATION
// ========================
// 1234
// RO27V14-XX29W5-6XFEDW-A0W935-81K

void SerialProcedure()
{
	if (GetWindowTextLengthW(TextboxUser) >= 3 && GetWindowTextLengthW(TextboxUser) <= 16 && GetWindowTextLengthW(TextboxSerial) == 32)
	{
		const WCHAR CHARS[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		WCHAR username[17], serial[33];
		int c = 0, all = 0, all2 = 0, lastNumber = -1;
		bool valid = false;

		EnableCharUpperWHook = true;

		vmemset(username, 0, sizeof(username));
		vmemset(serial, 0, sizeof(serial));

		GetDlgItemTextW(Hwnd, IDC_TEXTBOXUSER, username, _sizeof(username));
		GetDlgItemTextW(Hwnd, IDC_TEXTBOXSERI, serial, _sizeof(serial));

		CharUpperW(serial);

		// === SERIAL GENERATION ===

		for (int i = 0; i < _sizeof(username) && username[i] != '\0'; i++)
		{
			all += (((int)username[i] * i) + (all / (8 + i))) * i;
			all2 += (int)username[i];
		}

		if (all < 1000)
			all *= all;

		JUNK_CODE();

		all2 = all2 ^ lstrlenW(username);
		all2 = (15 * all2 + 1) / (15 + 1);
		while (all2 >= 36)
			all2 -= 7;

		JUNK_CODE();

		for (int i = 0; i < _sizeof(serial) - 1; i++)
		{
			if (i != 0 && i % 7 == 0)
			{
				valid = serial[i] == '-';
				if (!valid)
					break;
				else
					c++;
				continue;
			}

			JUNK_CODE();

			if (_sizeof(serial) % (2 + i) == 0 || i % 4 == 0)
			{
				DWORD RdtscLow = 0;

				WCHAR tempCode[] = { 0xBE, 0x83 };

				const int SIZE = _sizeof(tempCode) + 5;

				void* p;
				__asm
				{
					mov[p], offset StartJump1
				}

				DWORD jumpWay;
				DWORD oldProtection;
				LPVOID startAddr = p;
				LPVOID serialGenMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SIZE);
				VirtualProtect(serialGenMem, SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);
				VirtualProtect((CHAR*)startAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

				decrypt(tempCode, 0x59814, _sizeof(tempCode));

				for (int i = 0; i < _sizeof(tempCode); i++)
					vmemset((CHAR*)serialGenMem + i, (int)tempCode[i], 1);

				decrypt(tempCode, 0x59814, _sizeof(tempCode));

				vmemset((CHAR*)startAddr, 0xE9, 1);
				jumpWay = (DWORD)serialGenMem - ((DWORD)startAddr + 5);
				vmemcpy((CHAR*)startAddr + 1, &jumpWay, 4);
				vmemset((CHAR*)serialGenMem + _sizeof(tempCode), 0xE9, 1);
				jumpWay = ((DWORD)startAddr + 5) - ((DWORD)serialGenMem + SIZE);
				vmemcpy((CHAR*)serialGenMem + SIZE - 4, &jumpWay, 4);

			StartJump1:
				__asm // 5 bytes
				{
					jmp short StartJump1
					push eax
					jmp short StartJump1
				}

				__asm
				{
					mov RdtscLow, eax
				}

				HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, serialGenMem);

				int cint = 1337 + (all / (all2 + i));

				cint = ((3 / 2) * all) / cint;
				cint = cint ^ (i + 1);

				WCHAR temp[2];
				vmemset(temp, 0, sizeof(temp));
				temp[0] = serial[i];
				CharUpperW(temp);
				serial[i] = temp[0];

				JUNK_CODE();

				cint = (15 * cint + 1) / ((all2 / 2) + 1);

				if (lastNumber != -1 && lastNumber == cint)
					cint *= 2;

				while (cint >= 34)
					cint -= 2;

				while (cint < 0)
					cint += 2;

				lastNumber = cint;

				__asm
				{
					rdtsc
					sub eax, RdtscLow
					cmp eax, SERIAL_TIME
					jbe NotDebugged
					dec cint
				NotDebugged:
				}

				valid = serial[i] == CHARS[cint];
				if (!valid)
					break;
				else
					c++;
			}
			else
			{
				DWORD RdtscLow = 0;

				WCHAR tempCode[] = { 0xBE, 0x83 };

				const int SIZE = _sizeof(tempCode) + 5;

				void* p;
				__asm
				{
					mov[p], offset StartJump2
				}

				DWORD jumpWay;
				DWORD oldProtection;
				LPVOID startAddr = p;
				LPVOID serialGenMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SIZE);
				VirtualProtect(serialGenMem, SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);
				VirtualProtect((CHAR*)startAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

				decrypt(tempCode, 0x59814, _sizeof(tempCode));

				for (int i = 0; i < _sizeof(tempCode); i++)
					vmemset((CHAR*)serialGenMem + i, (int)tempCode[i], 1);

				decrypt(tempCode, 0x59814, _sizeof(tempCode));

				vmemset((CHAR*)startAddr, 0xE9, 1);
				jumpWay = (DWORD)serialGenMem - ((DWORD)startAddr + 5);
				vmemcpy((CHAR*)startAddr + 1, &jumpWay, 4);
				vmemset((CHAR*)serialGenMem + _sizeof(tempCode), 0xE9, 1);
				jumpWay = ((DWORD)startAddr + 5) - ((DWORD)serialGenMem + SIZE);
				vmemcpy((CHAR*)serialGenMem + SIZE - 4, &jumpWay, 4);

			StartJump2:
				__asm // 5 bytes
				{
					jmp short StartJump2
					push eax
					jmp short StartJump2
				}

				__asm
				{
					mov RdtscLow, eax
				}

				HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, serialGenMem);

				int cint = 1337 + (all / (35 - i));

				cint = ((3 / 2) * all) / cint;
				cint = cint % (i + 1);
				cint = (15 * cint + 1) / ((all2 / 2) + 1);

				WCHAR temp[2];
				vmemset(temp, 0, sizeof(temp));
				temp[0] = serial[i];
				CharUpperW(temp);
				serial[i] = temp[0];

				JUNK_CODE();

				if (lastNumber != -1 && lastNumber == cint)
					cint *= 2;

				while (cint > 36)
					cint -= 2;

				JUNK_CODE();

				while (cint < 0)
					cint += 2;

				lastNumber = cint;

				JUNK_CODE();

				__asm
				{
					rdtsc
					sub eax, RdtscLow
					cmp eax, SERIAL_TIME
					jbe NotDebugged2
					dec cint
				NotDebugged2:
				}

				valid = serial[i] == CHARS[cint];
				if (!valid)
					break;
				else
					c++;

				JUNK_CODE();
			}
		}

		EnableCharUpperWHook = false;

		if (valid)
		{
			JUNK_CODE();

			DWORD destination = (DWORD)Success - (DWORD)valid - (DWORD)c;

			WCHAR tempCode[] = { 0x28, 0x6C, 0x8C };

			const int SIZE = _sizeof(tempCode) + 5;

			void* p;
			__asm
			{
				mov [p], offset StartJump3
			}

			DWORD jumpWay;
			DWORD oldProtection;
			LPVOID startAddr = p;
			LPVOID serialGenMem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, SIZE);
			VirtualProtect(serialGenMem, SIZE, PAGE_EXECUTE_READWRITE, &oldProtection);
			VirtualProtect((CHAR*)startAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

			if (valid && c == 32)
				decrypt(tempCode, 0x59214, _sizeof(tempCode));

			for (int i = 0; i < _sizeof(tempCode); i++)
				vmemset((CHAR*)serialGenMem + i, (int)tempCode[i], 1);

			decrypt(tempCode, 0x59214, _sizeof(tempCode));

			vmemset((CHAR*)startAddr, 0xE9, 1);
			jumpWay = (DWORD)serialGenMem - ((DWORD)startAddr + 5);
			vmemcpy((CHAR*)startAddr + 1, &jumpWay, 4);
			vmemset((CHAR*)serialGenMem + _sizeof(tempCode), 0xE9, 1);
			jumpWay = ((DWORD)startAddr +5) - ((DWORD)serialGenMem + SIZE);
			vmemcpy((CHAR*)serialGenMem + SIZE - 4, &jumpWay, 4);

			__asm
			{
				mov eax, destination
			}

		StartJump3:
			__asm // 5 bytes
			{
				jmp short StartJump3
				push eax
				jmp short StartJump3
			}

			__asm
			{
				mov destination, eax
			}

			HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, serialGenMem);

			__asm
			{
				// call Success
				push end
				push destination
				retn
			end:
			}
		}
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_BUTTONREGIS)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					__asm
					{
						push end
						push SerialProcedure
						retn
					end:
					}
				}
			}

			if (LOWORD(wParam) == IDC_BUTTONABOUT)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					MessageBoxW(hwnd, L"KeygenMe V7\n========================\n"
						L"Release Date: 19. August 2014\n"
						L"Coded in C using Visual Studio 2013\n\n"
						L"View readme for more information!\n\n"
						L"Enjoy!\n"
						L"by MaxX0r // RTN\n"
						L"www.rtn-team.cc"
						, L"About", MB_ICONINFORMATION + MB_OK);
				}
			}

			if (LOWORD(wParam) == IDC_BUTTONEXIT)
			{
				if (HIWORD(wParam) == BN_CLICKED)
					ExitProcess(0);
			}

			if (LOWORD(wParam) == IDC_TEXTBOXUSER)
			{
				const WCHAR txtCheck[] = L"Username...";
				if (HIWORD(wParam) == EN_SETFOCUS)
				{
					WCHAR txtGet[_sizeof(txtCheck)];

					vmemset(txtGet, 0, sizeof(txtGet));

					GetDlgItemTextW(hwnd, IDC_TEXTBOXUSER, txtGet, _sizeof(txtGet));

					if (vwcsncmp(txtGet, txtCheck, _sizeof(txtCheck)) == 0)
						SetDlgItemTextW(hwnd, IDC_TEXTBOXUSER, L"");
				}

				if (HIWORD(wParam) == EN_KILLFOCUS)
				{
					if (GetWindowTextLengthW(TextboxUser) == 0)
						SetDlgItemTextW(hwnd, IDC_TEXTBOXUSER, txtCheck);
				}
			}

			if (LOWORD(wParam) == IDC_TEXTBOXSERI)
			{
				const WCHAR txtCheck[] = L"Serial...";
				if (HIWORD(wParam) == EN_SETFOCUS)
				{
					WCHAR txtGet[_sizeof(txtCheck)];

					vmemset(txtGet, 0, sizeof(txtGet));

					GetDlgItemTextW(hwnd, IDC_TEXTBOXSERI, txtGet, _sizeof(txtGet));

					if (vwcsncmp(txtGet, txtCheck, _sizeof(txtCheck)) == 0)
						SetDlgItemTextW(hwnd, IDC_TEXTBOXSERI, L"");
				}

				if (HIWORD(wParam) == EN_KILLFOCUS)
				{
					if (GetWindowTextLengthW(TextboxSerial) == 0)
						SetDlgItemTextW(hwnd, IDC_TEXTBOXSERI, txtCheck);
				}
			}
		}
		break;
	case WM_CREATE:
		{
			HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

			SendMessageW(hwnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(TRUE, 0));

			TextboxUser = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"Username...",
				WS_CHILD | WS_VISIBLE | ES_CENTER, 10, 15, 220, 20, hwnd, (HMENU)IDC_TEXTBOXUSER, GetModuleHandleW(NULL), NULL);
			SendMessageW(TextboxUser, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			TextboxSerial = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"Serial...",
				WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_CENTER, 10, 40, 220, 20, hwnd, (HMENU)IDC_TEXTBOXSERI, GetModuleHandleW(NULL), NULL);
			SendMessageW(TextboxSerial, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			ButtonRegister = CreateWindowExW(NULL, L"BUTTON", L"Register",
				WS_CHILD | WS_VISIBLE, 10, 70, 120, 26, hwnd, (HMENU)IDC_BUTTONREGIS, GetModuleHandleW(NULL), NULL);
			SendMessageW(ButtonRegister, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			ButtonAbout = CreateWindowExW(NULL, L"BUTTON", L"About",
				WS_CHILD | WS_VISIBLE, 135, 70, 45, 26, hwnd, (HMENU)IDC_BUTTONABOUT, GetModuleHandleW(NULL), NULL);
			SendMessageW(ButtonAbout, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			ButtonExit = CreateWindowExW(NULL, L"BUTTON", L"Exit",
				WS_CHILD | WS_VISIBLE, 185, 70, 45, 26, hwnd, (HMENU)IDC_BUTTONEXIT, GetModuleHandleW(NULL), NULL);
			SendMessageW(ButtonExit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			SendMessageW(TextboxUser, EM_LIMITTEXT, 16, 16);
			SendMessageW(TextboxSerial, EM_LIMITTEXT, 32, 32);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}
	return 0;
}

HWND WINAPI CreateWindowExWHook(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	// inject nag screen

#ifdef NDEBUG
	if (nWidth == 120)
	{
		INJECTED_DATA data;
		vmemset(&data, 0, sizeof(data));
		data.MessageBoxWPtr = (DWORD)MessageBoxW;
		data.Hwnd = Hwnd;

		WCHAR title[] = { 0x3B, 0x17, 0x10, 0x58, 0x2A, 0x19, 0x09, 0x19, 0x18, 0x10, 0x00 }; // Nag Screen
		WCHAR text[] = { 0x2B, 0x01, 0x09, 0x0A, 0x08, 0x44, 0x49, 0x23, 0x4B, 0x0D, 0x00, 0x4E, 0x0E, 0x50, 0x1F, 0x13, 0x14, 0x54, 0x06, 0x15, 0x05, 0x1D, 0x1C, 0x14, 0x5B, 0x1D,
			0x13, 0x1A, 0x5F, 0xC9, 0xA1, 0xE3, 0xEE, 0xA4, 0xED, 0xE3, 0xF5, 0xED, 0xA9, 0xE8, 0xEE, 0xEF, 0xEC, 0xFB, 0xFC, 0xF5, 0xB1, 0xEB, 0xFC, 0xE1, 0xE7, 0xB6,
			0xFB, 0xF1, 0xFA, 0xFF, 0xF5, 0xEF, 0xF8, 0xBE, 0xFA, 0xD8, 0xD1, 0xCB, 0xD1, 0xC1, 0xC1, 0x88, 0x00 }; // Hello, I am a nag screen and I am here because your license expired.
		decrypt(title, 0x93834, _sizeof(title) - 1);
		decrypt(text, 0x72834, _sizeof(text) - 1);

		vwcsncpy(data.Title, title, _sizeof(title));
		vwcsncpy(data.Text, text, _sizeof(text));

		DWORD addr = (DWORD)&data;
		__asm
		{
			// injectcode(&data);
			push addr
			push end
			push injectcode
			retn
		end:
		}
	}
#endif

	return OldCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	CRTInit();
	vInitCommonControls();

	MSG messages;
	WNDCLASSEXW wincl;
	RECT rect;
	int width = 250;
	int height = 130;
	WCHAR classname[] = L"keygenme.v7.maxx0r";

	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right/2) - (width/2);
	rect.top = (rect.bottom/2) - (height/2);

	wincl.hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON));
	wincl.hIconSm = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON));
	wincl.hInstance = hInstance;
	wincl.lpszClassName = classname;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(wincl);
	wincl.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClassExW(&wincl))
		return 1;
	else
	{
		DWORD hook1;
		__asm
		{
			push dword ptr CreateWindowExWHook
			push dword ptr CreateWindowExW
			push endh1
			push PrepareHook
			retn
		endh1:
			mov hook1, eax
		}
		OldCreateWindowExW = (CreateWindowExWPtr)hook1;

		DWORD hook2;
		__asm
		{
			push dword ptr CharUpperWHook
			push dword ptr CharUpperW
			push endh2
			push PrepareHook
			retn
		endh2:
			mov hook2, eax
		}
		OldCharUpperW = (CharUpperWPtr)hook2;

		//OldCreateWindowExW = (CreateWindowExWPtr)PrepareHook((CHAR*)CreateWindowExW, (DWORD)CreateWindowExWHook);
		//OldlstrlenW = (lstrlenWPtr)PrepareHook((CHAR*)lstrlenW, (DWORD)lstrlenWHook);
	}

	WCHAR windowTitle[] = { 0x70, 0x59, 0x44, 0x59, 0x5A, 0x2E, 0x0C, 0x27, 0x63, 0x12, 0x72, 0x66, 0x6A, 0x68, 0x1D, 0x38, 0x22, 0x2D, 0x21, 0x00 }; // KeygenMe V7 - Trial
	decrypt(windowTitle, 0x72014, _sizeof(windowTitle) - 1);

	Hwnd = CreateWindowExW(0, classname, windowTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, rect.left, rect.top, width, height,
		HWND_DESKTOP, NULL, hInstance, NULL);

	UpdateWindow(Hwnd);

	while (GetMessageW(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessageW(&messages);
	}

	return messages.wParam;
}