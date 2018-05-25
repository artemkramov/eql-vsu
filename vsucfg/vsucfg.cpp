// vsucfg.cpp : main source file for vsucfg.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <msi.h>

#include "resource.h"

#include "maindlg.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
#if 0
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
#else
	CMainDlg sheet;
	return sheet.DoModal();
#endif
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	{
		HMODULE hm=NULL;
		TCHAR szFile[MAX_PATH];
		LCID const lcid=1033;//GetThreadLocale();
		//GUID для vsucfg.exe
		TCHAR szComponent[]=TEXT("{90B2CB63-13DC-4C87-96AE-10EAD7687C37}");
		//определяем идентификатор продукта и установлен ли продукт
		TCHAR szProduct[39];
		UINT hr=MsiGetProductCode(szComponent,szProduct);
		if(hr==ERROR_SUCCESS){
			//в szProduct получен GUID продукта
			TCHAR szFeature[]=TEXT("vsuresXXXXXXXX");
			wsprintf(szFeature,TEXT("vsures%X"),lcid);
			wsprintf(szComponent,TEXT("{%08X-8BC5-41C2-9C60-D87F34A211F8}"),lcid);
			DWORD size=sizeof(szFile)/sizeof(szFile[0]);
			hr=MsiProvideComponent(szProduct,szFeature,szComponent,0,szFile,&size);
			if(hr==ERROR_SUCCESS){
				hm=LoadLibrary(szFile);
			}
		}
		if(hm==NULL){
			//продукт не установлен
			wsprintf(szFile,TEXT("resources\\%d\\vsucfgr.dll"),lcid);
			hm=LoadLibrary(szFile);
		}
		if(hm)
			_Module.m_hInstResource=hm;
	}

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
