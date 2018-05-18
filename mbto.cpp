#include "stdafx.h"
#include "mbto.h"

typedef void (CALLBACK *LPFN_CallWndRetProc) (WPARAM, LPARAM);

static LPFN_CallWndRetProc fnCallWndRetProc=NULL;
static UINT uElapse;
static bool timedout;

static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	timedout=true;
	KillTimer(hwnd,idEvent);
	HWND cur=NULL;
	while(cur=FindWindowEx(hwnd,cur,TEXT("Button"),NULL)){
		WINDOWINFO wi;
		wi.cbSize=sizeof(WINDOWINFO);
		if(GetWindowInfo(cur,&wi)){
			if(wi.dwStyle&BS_DEFPUSHBUTTON){
				SendMessage(cur,BM_CLICK,0,0);
				break;
			}
		}
	}
}

static void CALLBACK CallWndRetProc_Void(WPARAM wParam, LPARAM lParam)
{
}

static void CALLBACK CallWndRetProc_Init(WPARAM wParam, LPARAM lParam)
{
	PCWPRETSTRUCT data=PCWPRETSTRUCT(lParam);
	if(data->message==WM_INITDIALOG){
		SetTimer(data->hwnd,0,uElapse,TimerProc);
		fnCallWndRetProc=CallWndRetProc_Void;
	}
}

static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HC_ACTION){
		fnCallWndRetProc(wParam,lParam);
	}
	return CallNextHookEx(/*myhookdata[CALLWNDPROC].hhook*/NULL, nCode, wParam, lParam); 
}

struct hooker
{
	HHOOK hHook;
	hooker(DWORD dwMilliseconds):hHook(NULL)
	{
		timedout=false;
		uElapse=dwMilliseconds;
		fnCallWndRetProc=CallWndRetProc_Init;
		hHook=SetWindowsHookEx(WH_CALLWNDPROCRET,CallWndRetProc,NULL,GetCurrentThreadId());
	}
	~hooker()
	{
		if(hHook)
			UnhookWindowsHookEx(hHook);
	}
};

int WINAPI MessageBoxTimeout(IN HWND hWnd,IN LPCTSTR lpText, IN LPCTSTR lpCaption, 
		IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds)
{
	if(dwMilliseconds==INFINITE)
		return MessageBoxEx(hWnd,lpText,lpCaption,uType,wLanguageId);
	hooker hook(dwMilliseconds);
	int ret=MessageBoxEx(hWnd,lpText,lpCaption,uType,wLanguageId);
	return timedout?MB_TIMEDOUT:ret;
}

