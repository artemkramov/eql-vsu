#pragma once

#include <windows.h>
#include <winuser.h>

// Define replacement for undocumented User32 function MessageBoxTimeout which works only on XP and newer systems
// This implementation should work on W2K too

// возвращаемое значение если юзер не нажал на кнопки
#define MB_TIMEDOUT 32000

int WINAPI MessageBoxTimeout(IN HWND hWnd,IN LPCTSTR lpText, IN LPCTSTR lpCaption, 
		IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
