#include <windows.h>

BOOL WINAPI DllMainVsuErr(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason==DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hInstance);
	return TRUE;  // Successful
}
