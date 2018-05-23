// vsu.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f vsups.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "vsures\resource.h"
#include <msi.h>
#include <initguid.h>
#include "vsu.h"
#include <vector>

#include "vsu_i.c"
#include "HcUniverse.h"

#include <siv.h>
#include <handle.h>
#include "vsures\vsumc.h"
#include <sivdefs.h>
#include <boost/regex.hpp>
#include "md5.h"
#include "mbto.h"

#ifdef __LOGS__
app_slot mailslot;
CriticalSection logstream::logssect;
logstream::logset logstream::logs;
struct mailslotinitdone
{
	mailslotinitdone(){mailslot.init("CC");}
	~mailslotinitdone(){mailslot.done();}
};
#endif

extern std::ostringstream ve;
extern std::ostream &out;
#ifdef _DEBUG
extern int hcdb::bs_loggable;
#endif

char CExeModule::cnst_empty[2]="";
char CExeModule::cnst_left[2]="(";
char CExeModule::cnst_right[2]=")";

Co Co;

static CHAR strID[]=_T("1AE27B20-EAE0-11D3-9BE9-008048C5FDF8");
static CHAR classID[]=_T("VSUMessageWindow");
const DWORD dwTimeOutStd=5000;	//for embedded instances
const DWORD dwTimeOutForced=500;	//set when "shut down" or "terminate" pressed
static DWORD dwTimeOut = dwTimeOutStd; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up
static const UINT uTrayIcon=412; // icon id for Shell_NotifyIcon
static keo::FileHandle hOffFlag; // handle to "connection off" flag file

int const cyMul[]={10000,1000,100,10,1};

template <typename T>
LPTHREAD_START_ROUTINE tsr(DWORD (WINAPI T::* ptr_to_member)(void))
{
	return *reinterpret_cast<LPTHREAD_START_ROUTINE*>(&ptr_to_member);
}
 
HRESULT ShowError(HRESULT hr=S_OK)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM | 
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    hr==S_OK?(hr=GetLastError()):hr,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error in L2Srv", MB_OK|MB_ICONINFORMATION|MB_SETFOREGROUND|MB_TOPMOST);
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return hr;
}

class Timer
{
	HWND hWnd;
	UINT nIDEvent;
	UINT idTimer;
public:
	Timer(HWND ahWnd,UINT anIDEvent,UINT uElapse,TIMERPROC lpTimerProc):hWnd(ahWnd),nIDEvent(anIDEvent),idTimer(0)
	{
		idTimer=SetTimer(hWnd,nIDEvent,uElapse,lpTimerProc);
	}
	virtual ~Timer()
	{
		if(idTimer){
			if(hWnd)
				KillTimer(hWnd,nIDEvent);
			else
				KillTimer(hWnd,idTimer);
		}
	}
};

typedef CComPtr<IHcUniverse2> pIHcUniverse2;
typedef CComPtr<IHcUniverse3> pIHcUniverse3;
typedef CComPtr<IHcUniverse4> pIHcUniverse4;
typedef CComPtr<IHcUniverse5> pIHcUniverse5;
typedef CComPtr<IHcUniverse6> pIHcUniverse6;
// Для того, чтобы меньше исправлять тексты при появлении нового интерфейса
// везде, где требуется максимально свежий интерфейс, нужно использовать следующие typedef'ы:
typedef pIHcUniverse6 pIHcUniv;
typedef IHcUniverse6 IHcUniv;

template <typename T>
HRESULT GetItf(T**pp)
{
	//1. Ensure server is running

	//2. CreateInstance
	HRESULT hr=CoCreateInstance(CLSID_HcUniverse,NULL,CLSCTX_SERVER,__uuidof(T),(LPVOID*)pp);
	if(SUCCEEDED(hr)){
		if(_Module.OEMInputOverriden)
			hr=(*pp)->SetICP(_Module.OEMInput);
		if(SUCCEEDED(hr)){
			if(_Module.OEMOutputOverriden)
				hr=(*pp)->SetOCP(_Module.OEMOutput);
		}
	}
	return hr;
}

procedure&operator<<(procedure&proc,_variant_t&v){
	proc.fields.push_back(new field_value(proc.fields.size(),v));
	return proc;
}

procedure&operator<<(procedure&proc,unsigned __int64 v){
	_variant_t vcy;
	vcy.vt=VT_CY;
	vcy.cyVal.int64=v;
	vcy.cyVal.int64*=10000;
	return proc<<vcy;
}

procedure&operator<<(procedure&proc,double v){
	return proc<<_variant_t(v);
}

procedure&operator<<(procedure&proc,long v){
	return proc<<_variant_t(v);
}

procedure&operator<<(procedure&proc,int v){
	return proc<<long(v);
}

procedure&operator<<(procedure&proc,char const*v){
	_variant_t vbstr=_bstr_t(v);
	return proc<<vbstr;
}

procedure&operator<<(procedure&proc,std::string const&v){
	return proc<<_variant_t(v.c_str());
}

static inTimerProc=false;
static bool Waiter=false;
static bool Online=false;

VOID CALLBACK TimerProc(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT idEvent,  // timer identifier
  DWORD dwTime       // current system time
	)
{
	//Чтобы выдавало ошибки при отсутствии соединения, закомментирована проверка _Module.sess
	if(!inTimerProc&&/*_Module.sess&&*/(_Module.WatchDir.length()>0)&&!_Module.ShutDowned&&!_Module.Terminated){
		struct guard
		{
			guard(){inTimerProc=true;}
			~guard(){inTimerProc=false;}
		}g;
		enum ops
		{
			opFile,
			opForm,
			opComment,
			opClrAllPLU,
			opSqueeze,
			opReport,
			opFMReport,
			opCopyChk,
			opVoid,
		};
		struct descr
		{
			ops op;
			wchar_t const*fn;
		};
		descr const files[]={
			{opFile,L"File.vsu"},
			{opFile,L"File.dbf"},
			{opForm,L"Form.vsu"},
			{opForm,L"Form.dbf"},
			{opComment,L"Comment.vsu"},
			{opComment,L"Comment.dbf"},
			{opClrAllPLU,L"ClrAllPLU.vsu"},
			{opSqueeze,L"Squeeze.vsu"},
			{opReport,L"Report.vsu"},
			{opFMReport,L"FMReport.vsu"},
			{opCopyChk,L"CopyChk.vsu"},
			{opVoid,L"Void.vsu"},
			{opVoid,L"Void.dbf"},
		};
#pragma message("Non portable: В поле типа unsigned __int32 может храниться указатель")
		typedef std::map<unsigned __int64,unsigned __int32> todo_t;
		todo_t todo;
		bool block = false;
		std::string block_log_name;
		char repno[16]="";
		char fmrep[128]="";
		char*fmrep1=NULL;
		char*fmrep2=NULL;
		char*fmrep3=NULL;
		char*fmrep4=NULL;
		char*fmrep5=NULL;
		for(int i=0;i<A_SIZE(files);i++){
			_bstr_t fn=_Module.WatchDir+files[i].fn;
			DWORD fattrs=GetFileAttributes(fn);
			if(fattrs!=0xffffffff){
				//файл существует
				if(GetFileAttributes(fn+L".log")!=0xffffffff){
					//log exists
					if(_Module.ErrMode == 1){
						//log exists and alternative functionality enforced
						//we need to stop and wait for user to remove log with error
						block = true;
						block_log_name = fn+L".log";
					}
					continue;
				}
				keo::FileHandle h=CreateFile(fn,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(h!=INVALID_HANDLE_VALUE){
					FILETIME CreationTime,LastAccessTime,LastWriteTime;
					if(GetFileTime(h,&CreationTime,&LastAccessTime,&LastWriteTime)){
						todo[*reinterpret_cast<unsigned __int64*>(&LastWriteTime)]=i;
						if(files[i].op==opReport){
							DWORD BytesRead;
							if(ReadFile(h,repno,sizeof(repno)-1,&BytesRead,NULL)){
								repno[BytesRead]=0;
								char*p=repno;
								while('0'<=*p&&*p<='9')
									p++;
								*p=0;
							}
						}
						else if(files[i].op==opFMReport){
							DWORD BytesRead;
							if(ReadFile(h,repno,sizeof(fmrep)-1,&BytesRead,NULL)){
								fmrep[BytesRead]=0;
								char const*seps=" \t\r\n";
								fmrep1=strtok(fmrep,seps);
								if(fmrep1)
									fmrep2=strtok(NULL,seps);
								if(fmrep2)
									fmrep3=strtok(NULL,seps);
								if(fmrep3)
									fmrep4=strtok(NULL,seps);
								if(fmrep4)
									fmrep5=strtok(NULL,seps);
							}
						}
					}
				}
			}
		}
		if(_Module.WatchMask.length()>0){
			_bstr_t wfn=_Module.WatchDir+_Module.WatchMask;
			using namespace keo;
			WIN32_FIND_DATA wfd;
			FindHandle h;
			h=FindFirstFile(wfn,&wfd);
			if(h!=INVALID_HANDLE_VALUE){
				do{
					if((wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0){
						//файл существует
						_bstr_t fn=_Module.WatchDir+wfd.cFileName;
						if(GetFileAttributes(fn+L".log")==0xffffffff){
							//лог не существует
							FileHandle h=CreateFile(fn,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
							if(h!=INVALID_HANDLE_VALUE){
								FILETIME CreationTime,LastAccessTime,LastWriteTime;
								if(GetFileTime(h,&CreationTime,&LastAccessTime,&LastWriteTime)){
									wchar_t*buf=new wchar_t[_tcslen(wfd.cFileName)+1];
									wcscpy(buf,_bstr_t(wfd.cFileName));
									todo[*reinterpret_cast<unsigned __int64*>(&LastWriteTime)]=reinterpret_cast<unsigned __int32>(buf);
								}
							}
						}
						else if(_Module.ErrMode==1){
							//log exists and alternative functionality enforced
							//we need to stop and wait for user to remove log with error
							block = true;
							block_log_name = fn+L".log";
						}
					}
				}
				while(FindNextFile(h,&wfd));
			}
		}
		// block can be true only if ErrMode is 1 (and ErrMode can be 1 only if alterfunc is 1)
		if(block){
			//skip all operations because log file with error exists
			//free strings
			for(todo_t::const_iterator it = todo.begin(); it != todo.end(); ++it){
				if(it->second >= A_SIZE(files))
					delete[]reinterpret_cast<wchar_t*>(it->second);
			}
			//show specified message
			std::stringstream msg;
			{
				msg << TEXT("ХРБСЧЕЙ УЦБЛМБ. ЕЛЕГОФЕ УФПН ЦБКЕЛП ЛЗШЗУ БСЧЕЙЩН ФП LOG FILE");
				std::string line;
				std::ifstream ifs(block_log_name.c_str());
				std::getline(ifs, line); // skip first line of log file
				while(ifs){
					std::getline(ifs, line);
					msg << '\n' << line;
				}
			}
			UINT uiFlags = MB_SETFOREGROUND|MB_SYSTEMMODAL|MB_ICONINFORMATION;
			int mbTimeout = 5000;
			if (_Module.RepeatMode) {
				uiFlags |= MB_RETRYCANCEL;
				mbTimeout = INFINITE;
			}
			else {
				uiFlags |= MB_OK;
			}
			int result = MessageBoxTimeout(NULL, msg.str().c_str(), TEXT("VSU warning"), uiFlags, 0, mbTimeout);
			
			// If the user pressed 'Retry' button
			// than remove log file and allow remaking of the operation
			if (result == IDRETRY) {
				remove(block_log_name.c_str());	
			}
		}
		else //standard vsu file mode
		if(!todo.empty()){
			pIHcUniv p;
			HRESULT hr=GetItf(&p);
			if(p&&SUCCEEDED(hr)){
				extern universes_set universes;
				CHcUniverse*pcu;
				for(universes_set::const_iterator uit=universes.begin();uit!=universes.end();uit++){
					IHcUniv* tmp=*uit;
					if(tmp==p.p){
						pcu=*uit;
						break;
					}
				}
				struct guard
				{
					CHcUniverse*pcu;
					_bstr_t finp;
					_bstr_t slog;
					HRESULT hr;
					guard(CHcUniverse*apcu, _bstr_t const& inp, _bstr_t const& s):pcu(apcu),finp(inp),slog(s),hr(S_OK)
					{
						pcu->TmpLogFile=static_cast<char*>(slog);
					}
					~guard()
					{
						pcu->TmpLogFile=std::string();
						if(_Module.ErrMode != 1 || hr != S_OK){
							rename(slog,slog+"og");
						}
						else{
							DeleteFile(finp);
							DeleteFile(slog);
						}
					}
				};
				for(todo_t::const_iterator it=todo.begin();it!=todo.end()&&SUCCEEDED(hr);it++){
					struct freestr
					{
						wchar_t *str;
						freestr():str(NULL){}
						~freestr(){if(str)delete[]str;}
					}freestr;
					descr ldescr={opFile};
					descr const*d=NULL;
					if(it->second<A_SIZE(files))
						d=files+it->second;
					else{
						ldescr.fn=freestr.str=reinterpret_cast<wchar_t*>(it->second);
						d=&ldescr;
					}
					_bstr_t fn=_Module.WatchDir+d->fn;
					_bstr_t fnl=fn+L".l";
					if(_Module.sess && Online){
						switch(d->op){
						case opFile:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->ProcessFile(fn);
							}
							break;
						case opForm:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->PrintForm(fn);
							}
							break;
						case opComment:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->PrintComment(fn);
							}
							break;
						case opClrAllPLU:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->ClrAllPlu();
							}
							break;
						case opSqueeze:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->Squeeze();
							}
							break;
						case opReport:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->Call1(L"PrintReport",_variant_t(repno));
							}
							break;
						case opFMReport:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->Call5(L"FMReport",_variant_t(fmrep1),_variant_t(fmrep2),_variant_t(fmrep3),_variant_t(fmrep4),_variant_t(fmrep5));
							}
							break;
						case opCopyChk:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->Call0(L"CopyChk");
							}
							break;
						case opVoid:
							{
								guard g(pcu, fn, fnl);
								g.hr = hr = p->PrintVoid(fn);
							}
							break;
						}
					}
					else{
						//Просто выводим ошибку о том, что соединение ещё не установлено
						//Нужно вычеркнуть из условия в начале этой функции проверку _Module.sess
						//Такое поведение приведёт к некоторому изменению поведения по сравнению
						//со старыми версиями VSU (надеемся, что до несовместимости дело не дойдёт)
						guard g(pcu, fn, fnl);
						g.hr = hr = p->PrintError(fn,_Module.sess?st_temporary_unavailable:Waiter?st_no_session_established:st_no_session_started);
					}
				}
			}
		}
	}
}

// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

//Thread procedure to execute configurator and wait for its termination
DWORD WINAPI CExeModule::VSUCfg()
{
	try{
		_bstr_t cmd=hvsu[_T("VSUCfg")];
		STARTUPINFO sui;
		memset(&sui,0,sizeof(sui));
		sui.cb=sizeof(sui);
		PROCESS_INFORMATION pi;
		memset(&pi,0,sizeof(pi));
		if(CreateProcess(NULL,cmd,NULL,NULL,FALSE,0,NULL,NULL,&sui,&pi)){
			EnableMenuItem(hMenu,ID_M1_OPTIONS,MF_GRAYED);
			HANDLE wfmo[2]={pi.hProcess,hMonitorThread};
			switch(WaitForMultipleObjects(A_SIZE(wfmo),wfmo,FALSE,INFINITE)){
			case WAIT_OBJECT_0:
				DWORD ExitCode=0;
				if(GetExitCodeProcess(pi.hProcess,&ExitCode))
					if(ExitCode)
						ExecuteRegConfiguration();
				EnableMenuItem(hMenu,ID_M1_OPTIONS,MF_ENABLED);
				break;
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	catch(hkey::regerror&){
	}
	return 0;
}

CExeModule::CExeModule():
#if defined(ALTERFUNC)
	alterfunc(ALTERFUNC),
#else
	alterfunc(0),
#endif
	Format(0),
	useFormat(false),
	ErrMode(alterfunc==1 ? 1 : 0),
	RepeatMode(true)
{
	long lang=GetThreadLocale();
	try{
		lang=hkey(hvsu,TEXT("lang"))[TEXT("")];
	}
	catch(hkey::regerror&){}//если в реестре нет того, что нам нужно
	catch(_com_error&){}//если из реестра прочитано значение, которое не м. б. преобразовано в long
	eres=LoadResourceModule(1033);
	hres=LoadResourceModule(lang);
	if(eres==NULL&&hres==NULL)
			MessageBox(NULL,TEXT("VSU cannot load resources. Please repair or reinstall VSU."),NULL,MB_OK|MB_ICONSTOP|MB_SETFOREGROUND|MB_TOPMOST);
}

LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0)
    {
        bActivity = true;
        SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
    }
    return l;
}

//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
    while (1)
    {
        WaitForSingleObject(hEventShutdown, INFINITE);
        DWORD dwWait=0;
        do
        {
            bActivity = false;
            dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
        } while (dwWait == WAIT_OBJECT_0);
        // timed out
        if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
        {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
            CoSuspendClassObjects();
            if (!bActivity && m_nLockCnt == 0)
#endif
                break;
        }
    }
    CloseHandle(hEventShutdown);
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    hMonitorThread = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (hMonitorThread != NULL);
}

hkey hvsu(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Help Co\\vsu"));
CExeModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_HcUniverse, CHcUniverse)
END_OBJECT_MAP()


LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
	)
{
	return _Module.WindowProc(hwnd,uMsg,wParam,lParam);
}
LRESULT CALLBACK CExeModule::WindowProc(
	HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
	)
{
	if(uMsg==msgID){
		if(bEmbedding){
			MakeStandAlone();
		}
	}
	else if(uMsg==WM_USER){
		_ASSERTE(wParam==uTrayIcon);
		POINT pt;
		GetCursorPos(&pt);
		switch(lParam){
		case WM_LBUTTONDBLCLK:
			if(ShutDowned)
				MenuTerminate();
			else
				MenuShutDown();
			break;
		case WM_RBUTTONUP:
			if(ShutDowned)
				break;
			SetForegroundWindow(hwnd);
			// Display the menu
			switch(TrackPopupMenu(
				GetSubMenu(hMenu,0),
				TPM_RETURNCMD|TPM_NONOTIFY|TPM_RIGHTALIGN|TPM_BOTTOMALIGN,
				pt.x,
				pt.y,
				0,
				hwnd,
				NULL)){
			case ID_M1_SHUTDOWN:
				MenuShutDown();
				break;
			case ID_M1_TERMINATE:
				MenuTerminate();
				break;
			case ID_M1_ZREP:
				PostMessage(hwnd,WM_USER+1,0,0);
				break;
			case ID_M1_TREP:
				PostMessage(hwnd,WM_USER+1,0,1);
				break;
			case ID_M1_XREP:
				PostMessage(hwnd,WM_USER+1,0,10);
				break;
			case ID_M1_PREP:
				PostMessage(hwnd,WM_USER+1,0,20);
				break;
			case ID_M1_OPTIONS:
				{
					DWORD dwThreadId;
					CloseHandle(CreateThread(NULL,0,tsr(&CExeModule::VSUCfg),this,0,&dwThreadId));
				}
				break;
			}
			PostMessage(hwnd, WM_NULL, 0, 0);
			break;
		}
	}
	else if(uMsg==WM_USER+1){
		TCHAR s[16];
		wsprintf(s,TEXT("%lu"),lParam);
		HRESULT hres=CallCall1(s,TEXT("PrintReport"));
		if(lParam == 0 && hres == 0xC00051d7){
			int ret=MessageBox(NULL,
				TEXT("ЗМ. УХНПЛБ МЗДЕНЙКБ ГЙБ ЕКДПУЗ Ж РБФЗУФЕ ФП РЛЗКФСП (НБЙ)\nГЙБ МЗ ЕКДПУЗ Ж РБФЗУФЕ ФП РЛЗКФСП (ПЧЙ)"),
				TEXT("VSU"),
				MB_YESNO|MB_SETFOREGROUND|MB_TOPMOST|MB_ICONQUESTION);
			if(ret==IDYES){
				CallCall1("100",TEXT("PrintReport"));
			}
		}
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

void InitVersionMenuItem(HMENU hMenu)
{
	//GUID для vsu.exe
	TCHAR szComponent[]=TEXT("{1E55EC5A-5CC9-494D-8531-F0E0F97FEA89}");
	//определяем идентификатор продукта и установлен ли продукт
	TCHAR szProduct[39];
	UINT hr=MsiGetProductCode(szComponent,szProduct);
	if(hr==ERROR_SUCCESS){
		//в szProduct получен GUID продукта
		TCHAR szVersion[25+6]=TEXT("VSU v.");
		DWORD dwVersion=A_SIZE(szVersion)-6;
		hr=MsiGetProductInfo(szProduct,INSTALLPROPERTY_VERSIONSTRING,szVersion+6,&dwVersion);
		if(ERROR_SUCCESS==hr){
			AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
			AppendMenu(hMenu,MF_STRING|MF_DISABLED,0,szVersion);
		}
	}
}

void InstallTrayIcon(HINSTANCE hInstance)
{
//LoadMenu
	_Module.hMenu=LoadMenu(hInstance,MAKEINTRESOURCE(IDR_TrayMenu));
	SetMenuDefaultItem(GetSubMenu(_Module.hMenu,0),ID_M1_SHUTDOWN,FALSE);
	InitVersionMenuItem(GetSubMenu(_Module.hMenu,0));
//Shell_NotifyIcon
	_Module.nid.cbSize=sizeof(_Module.nid);
	_Module.nid.hWnd=_Module.hwnd;
	_Module.nid.uID=uTrayIcon;
	_Module.nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	_Module.nid.uCallbackMessage=WM_USER;
	_Module.nid.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_RED));
	LoadString(hInstance,IDS_RED,_Module.nid.szTip,A_SIZE(_Module.nid.szTip));
	BOOL bol=Shell_NotifyIcon(NIM_ADD,&_Module.nid);
	_ASSERTE(bol);
	hOffFlag=CreateFile(TEXT("no-conn.flg"), 0, 0, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL);
}

void CExeModule::ModifyTrayIcon(mtiState state)
{
	UINT uEnable=MF_GRAYED;
	nid.uFlags=NIF_ICON|NIF_TIP;
	switch(state){
	case mtiRed:
		if(!keo::ValidFileHandle(hOffFlag))
			hOffFlag=CreateFile(TEXT("no-conn.flg"), 0, 0, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL);
		nid.hIcon=LoadIcon(m_hInstResource,MAKEINTRESOURCE(IDI_RED));
		LoadString(m_hInstResource,IDS_RED,nid.szTip,A_SIZE(nid.szTip));
		break;
	case mtiYellow:
		if(!keo::ValidFileHandle(hOffFlag))
			hOffFlag=CreateFile(TEXT("no-conn.flg"), 0, 0, NULL, CREATE_ALWAYS, FILE_FLAG_DELETE_ON_CLOSE, NULL);
		nid.hIcon=LoadIcon(m_hInstResource,MAKEINTRESOURCE(IDI_YELLOW));
		LoadString(m_hInstResource,IDS_YELLOW,nid.szTip,A_SIZE(nid.szTip));
		break;
	case mtiGreen:
		taxes.clear();
		hOffFlag.Close();
		nid.hIcon=LoadIcon(m_hInstResource,MAKEINTRESOURCE(IDI_GREEN));
		LoadString(m_hInstResource,IDS_GREEN,nid.szTip,A_SIZE(nid.szTip));
		uEnable=MF_ENABLED;
		break;
	}
	BOOL bol=Shell_NotifyIcon(NIM_MODIFY,&nid);
	_ASSERTE(bol);
	EnableMenuItem(hMenu,ID_M1_ZREP,uEnable);
	EnableMenuItem(hMenu,ID_M1_XREP,uEnable);
	EnableMenuItem(hMenu,ID_M1_TREP,uEnable);
	EnableMenuItem(hMenu,ID_M1_PREP,uEnable);
}

// NIF_INFO is not supported by the windows system include files used to build VSU

//void CExeModule::TrayIconBalloon(LPCTSTR szInfo, LPCTSTR szInfoTitle, DWORD dwFlags)
//{
//	nid.uFlags = NIF_INFO;
//	nid.szInfo[0] = 0;
//	nid.szInfoTitle[0] = 0;
//	nid.dwInfoFlags = dwFlags;
//	nid.uTimeout = 0;
//	BOOL bol = Shell_NotifyIcon(NIM_MODIFY, &nid);
//	_ASSERTE(bol);
//	if(szInfo){
//		lstrcpyn(nid.szInfo, szInfo, 200);
//		if(szInfoTitle){
//			lstrcpyn(nid.szInfoTitle, szInfoTitle, 48);
//		}
//		nid.dwInfoFlags = dwFlags;
//		nid.uTimeout = 0;
//		BOOL bol = Shell_NotifyIcon(NIM_MODIFY, &nid);
//		_ASSERTE(bol);
//	}
//}

void UninstallTrayIcon()
{
	BOOL bol=Shell_NotifyIcon(NIM_DELETE,&_Module.nid);
	_ASSERTE(bol);
	if(_Module.hMenu)
		DestroyMenu(_Module.hMenu);
	hOffFlag.Close();
}

// вэйтеры создаются по аналогии с функцией session_connector из файла classes.cpp
static void vsu_set_waiter(session*s)
{
	s->set_waiter(
		new user_waiter2("VSU","Waiting for connect"),
		new user_waiter2("VSU","Waiting for device reconnect"),
		new user_waiter2("VSU","Waiting for data from device"));
}
static void vsu_set_online_waiter(session*s)
{
	s->set_online_wait(
		new user_waiter(500,"VSU (online thread)","Waiting for connect"),
		new user_waiter(500,"VSU (online thread)","Waiting for device reconnect"),
		new time_waiter(50,50));
}
size_t isParam(const _TCHAR * buf,const _TCHAR * pattern)
{
	size_t plen=_tcslen(pattern);
	if(_tcsnicmp(buf,pattern,plen)==0)
		if(buf[plen]==0||buf[plen]==_T(' '))
			return plen+1;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(
	HINSTANCE hInstance, 
	HINSTANCE /*hPrevInstance*/,
	LPTSTR lpCmdLine,
	int /*nShowCmd*/)
{
	if(_Module.hres==NULL&&_Module.eres==NULL)
		return 13;
#ifdef __LOGS__
	mailslotinitdone msid;
#endif
	set_waiter_func=vsu_set_waiter;
	set_online_waiter_func=vsu_set_online_waiter;
#ifdef _DEBUG
	hcdb::bs_loggable=_CRT_WARN;
#endif

	_Module.main(hInstance,lpCmdLine);

	lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

	_Module.Init(ObjectMap, hInstance, &LIBID_VSULib);
	_Module.dwThreadID = GetCurrentThreadId();
	_Module.hMenu=NULL;
	TCHAR szTokens[] = _T("-/");

	int nRet = 0;
	BOOL bRun = TRUE;
	LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
	_Module.Binary=false;
	_Module.bEmbedding=false;
	size_t isP;
	while (lpszToken != NULL)
	{
		if(lpszToken[0]==_T('A')){
			if(lpszToken[1]==_T('u')){
				if(lpszToken[2]==_T('t')){
					if(lpszToken[3]==_T('h')){
						if(lpszToken[4]==_T('o')){
							if(lpszToken[5]==_T('r')){
								if(lpszToken[6]==_T(' ')||lpszToken[6]==0){
									_Module.Author=true;
								}
							}
						}
					}
				}
			}
		}
		if(_tcsnicmp(lpszToken,_T("WinInput "),9)==0||lstrcmpi(lpszToken, _T("WinInput"))==0){
			_Module.OEMInput=false;
			_Module.OEMInputOverriden=true;
		}
		if(_tcsnicmp(lpszToken,_T("WinOutput "),10)==0||lstrcmpi(lpszToken, _T("WinOutput"))==0){
			_Module.OEMOutput=false;
			_Module.OEMOutputOverriden=true;
		}
		if(_tcsnicmp(lpszToken,_T("Binary "),7)==0||lstrcmpi(lpszToken, _T("Binary"))==0){
			_Module.Binary=true;
		}
		if(_tcsnicmp(lpszToken,_T("Connect "),8)==0){
			nRet=_Module.CallConnect(lpszToken+8);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("File "),5)==0){
			nRet=_Module.CallFile(lpszToken+5);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Form "),5)==0){
			nRet=_Module.CallForm(lpszToken+5);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Comment "),8)==0){
			nRet=_Module.CallComment(lpszToken+8);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Void "),5)==0){
			nRet=_Module.CallVoid(lpszToken+5);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Report "),7)==0){
			nRet=_Module.CallCall1(lpszToken+7,_T("PrintReport"));
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("FMReport "),9)==0){
			nRet=_Module.CallCall5(const_cast<LPTSTR>(lpszToken)+9,_T("FMReport"));
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Format "),7)==0){
			nRet=_Module.CallFormat(lpszToken+7);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Wildcard "),9)==0){
			nRet=_Module.CallWildcard(lpszToken+9);
			bRun = FALSE;
			break;
		}
		if(_tcsnicmp(lpszToken,_T("Watch "),6)==0){
			nRet=_Module.CallWatch(lpszToken+6);
			bRun = FALSE;
			break;
		}
		isP=isParam(lpszToken,_T("ClrAllPLU"));
		if(isP){
			nRet=_Module.CallCAP(lpszToken+isP);
			bRun = FALSE;
			break;
		}
		isP=isParam(lpszToken,_T("CopyChk"));
		if(isP){
			nRet=_Module.CallCall0(lpszToken+isP,_T("CopyChk"));
			bRun = FALSE;
			break;
		}
		isP=isParam(lpszToken,_T("Squeeze"));
		if(isP){
			nRet=_Module.CallSqueeze(lpszToken+isP);
			bRun = FALSE;
			break;
		}
		isP=isParam(lpszToken,_T("Unload"));
		if(isP){
			nRet=_Module.CallUnload(lpszToken+isP);
			bRun = FALSE;
			break;
		}
		isP=isParam(lpszToken,_T("UnregServer"));
		if (isP)
		{
				_Module.UpdateRegistryFromResource(IDR_Vsu, FALSE);
				nRet = _Module.UnregisterServer(TRUE);
				if(FAILED(nRet))
					ShowError(nRet);
				bRun = FALSE;
				break;
		}
		isP=isParam(lpszToken,_T("RegServer"));
		if (isP)
		{
				_Module.UpdateRegistryFromResource(IDR_Vsu, TRUE);
				nRet = _Module.RegisterServer(TRUE);
				if(FAILED(nRet))
					ShowError(nRet);
				bRun = FALSE;
				break;
		}
		isP=isParam(lpszToken,_T("Embedding"));
		if(isP)
		{
			_Module.bEmbedding=true;
		}
		lpszToken = FindOneOf(lpszToken, szTokens);
	}

	if (bRun)
	{
		_Module.InitOEMs();
		_Module.msgID=RegisterWindowMessage(strID);
		_ASSERTE(_Module.msgID);

		Handle hMutex=CreateMutex(NULL,FALSE,strID);
		_ASSERTE(hMutex);

		DWORD dw=WaitForSingleObject(hMutex,INFINITE);
		_ASSERTE(dw==WAIT_OBJECT_0);

		HWND existinghwnd=FindWindow(classID,strID);
		if(existinghwnd){
			//процесс уже существует
			ReleaseMutex(hMutex);
			if(!_Module.bEmbedding){
				//посылаем сообщение о том что запущено без embedding
				PostMessage(existinghwnd,_Module.msgID,0,0);
			}
		}
		else{
			HRESULT hRes;
			//запускаем процесс (and creating window)
			_Module.BeforeRun(hInstance);
			ReleaseMutex(hMutex);
			if(!_Module.bEmbedding){
				_Module.Lock();
				InstallTrayIcon(_Module.m_hInstResource);
			}
			
			_Module.StartMonitor();
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
			hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,REGCLS_MULTIPLEUSE|REGCLS_SUSPENDED);
			_ASSERTE(SUCCEEDED(hRes));
			hRes = CoResumeClassObjects();
#else
			hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,REGCLS_MULTIPLEUSE);
#endif
			_ASSERTE(SUCCEEDED(hRes));
			
			regLong uElapse(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu"),_T("AutoCheckInterval"),500);
			Timer t(NULL,0,long(uElapse),&TimerProc);
			//Create message queue (as described in PostThreadMessage topic in MSDN
			MSG msg;
			PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

			//Read registry configuration
			_Module.ExecuteRegConfiguration();

			_RPT1(_CRT_WARN,"Starting message cycle in thread %08x\n",GetCurrentThreadId());
			while(GetMessage(&msg, 0, 0, 0)){
				_RPT1(_CRT_WARN,"msg.message=%#08x\n",msg.message);
				DispatchMessage(&msg);
			}
			
			_Module.RevokeClassObjects();
			if(!_Module.bEmbedding){
				UninstallTrayIcon();
			}
			DestroyWindow(_Module.hwnd);
			Sleep(dwPause); //wait for any threads to finish
			
		}
	}

	_Module.Term();
#ifdef _DEBUG
	_RPT1(_CRT_WARN,"Returning %#010x\n",nRet);
#endif
	return nRet;
}

void CExeModule::main(HINSTANCE hInstance,LPTSTR lpCmdLine)
{
	srand((unsigned)time(NULL));
	Author=false;
	sess=NULL;
	sh=NULL;
	OEMInputOverriden=false;
	OEMOutputOverriden=false;
}

#ifndef HARD_LOCK
#include "kg.h"
algpar ap;//параметры алгоритма шифрации (инициализация в CExeModule::Register)

#define num4(aa) double(aa)

__forceinline bool Authorize(session*sess,unsigned datalen,keyc*data)
{
	//первым делом проверяем наличие процедуры авторизации в схеме
	//если ее нет - авторизация прошла успешно
	//если она есть проверяем длину данных
	//если длина достаточна, то вызываем процедуру авторизации
	//если длина недостаточна, то авторизация не прошла
	shema const*sh=sess->get_shema();
	//В аппаратах ПФП-1 (схемы 00180010, 00180011, 00180100, 00180101)
	//есть процедура авторизации, но не сохранились файлы rpt с паролями
	//Поэтому для этих аппаратов мы пропускаем процедуру авторизации:
	{
		union{
			unsigned long device_id;
			struct{
				unsigned long d1:4;
				unsigned long d2:4;
				unsigned long d3:4;
				unsigned long d4:4;
				unsigned long d5:4;
				unsigned long d6:4;
				unsigned long d7:4;
				unsigned long d8:4;
			};
		};
		int size_id=sizeof(device_id);
		sh->get_device_id(&device_id,&size_id);
		if(size_id==4){
			if(d8==d7&&d7==d4&&d4==0){
				if(d5==8&&d6==1){
					if(d1==0){
						if(d2==0){
							return d3==1;
						}
						else if(d2==1){
							return d3==0;
						}
					}
					else if(d1==1){
						if(d2==0){
							return d3==1;
						}
						else if(d2==1){
							return d3==0;
						}
					}
				}
			}
		}
	}
	TABL::pcrec it=sh->tabl.begin();
	bool found=false;
	while(sh->tabl.valid(it)){
		if(it->NAME.size()==9){
			if(it->NAME.substr(6,3)=="ize"){
				if(it->NAME.at(1)=='u'){
					if(it->NAME.substr(2,3)=="tho"){
						if(it->NAME[5]=='r'){
							if(it->NAME.at(0)=='A'){
								//процедура авторизации найдена
								found=true;
								TYPE::pcrec tit=sh->type.type(it->ID);
								if(datalen>=tit->SIZE){
									author_t ad;
									if(check_sernum(ap,sizeof(author_t),&ad,datalen,data,data)){
										procedure proc(it->NAME);
										proc.fields.push_back(new field_value("Password1",num4(ad.psw1)));
										proc.fields.push_back(new field_value("Password2",num4(ad.psw2)));
										proc.fields.push_back(new field_value("Seed",num4(ad.seed)));
										pgsingle_row sr=proc.result_row=new single_row(sh);
										ccerr::exec(sess,&proc);
										if(!memcmp(sr->column_ptr("Info"),ad.answer,sizeof(ad.answer))){
											return true;
										}
									}
								}
								return false;
							}
						}
					}
				}
			}
		}
		it++;
	}
	return !found;
}
#endif

#ifndef HARD_LOCK
__forceinline bool ScanKeysFile(session*sess,double vdate)
{
	using namespace std;
	_TCHAR kfn[256]="";
	keysfile(kfn,sizeof(kfn));
	if(*kfn){
		ifstream ifs(kfn);
		char buf[1024];
		while(ifs){
			ifs.getline(buf,sizeof(buf));
			keyc data[256];
			unsigned datalen=decode64(ap,buf,data);
			sernum_t sn;
			if(check_sernum(ap,sizeof(sn),&sn,datalen,data,data)){
				if(sn.sernum==sess->ser_num().number){
					unsigned long date;
					if(check_sernum(ap,sizeof(date),&date,datalen-sizeof(sn)-1,data,data)){
						union{
							USHORT vd;
							struct{
								USHORT vdd:5;
								USHORT vdm:4;
								USHORT vdy:7;//add 1980 to get actual year
							};
						};
						USHORT vt;
						VariantTimeToDosDateTime(vdate,&vd,&vt);
						if(((vdy+1980)*100+vdm)*100+vdd<=date){
							return Authorize(sess,datalen-sizeof(sn)-1-sizeof(date)-1,data);
						}
					}
				}
			}
		}
	}
	return false;
}
#endif

__forceinline bool CExeModule::FreeSchema402()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
		struct{
			unsigned __int32 version:12;
			unsigned __int32 model:12;
			unsigned __int32 mfgctry:8;
		};
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Болгарии
	case 0x0b:
		switch(model){
		case 0x420://0b420001 Orgtech 2000 EL
		case 0x429://0b429001
		case 0x712://0b712001
		case 0x812://0b812001 OT 812
			if((version&0xFFC)==0)
				break;
			return false;
		case 0xf42://0bf42901
			if((version&0xFFC)==0x900)
				break;
		default:
			return false;
		}
		break;
  //новые
	case 0xD4:
		switch(ui8[2]){
		case 0x00://D4000001
		case 0x01://D4010001
		case 0x20://D4200001
		case 0x21://D4210001
		case 0x32://D4320001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xD7:
		switch(ui8[2]){
		case 0x43://D7430001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xD8:
		switch(ui8[2]){
		case 0x12://D8120001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	//были раньше (в серию не пошли, но есть у болгар)
	case 0xE4:
		switch(ui8[2]){
		case 0x00://E4000001    E4000002
		case 0x20://E4200001    E4200002
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xE7:
		switch(ui8[2]){
		case 0x43://E7430001    E7430002
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

// 20120711: по просьбе болгар следующие схемы молдавских кассовых аппаратов должны требовать ключи:
// 0x0E200001 +
// 0x0E401001 +
// 0x0E411A01 +
// 0x0E412B01 +
// 0x0E412001 +
// 0x0E712001 +
// 0xE4010003 +
__forceinline bool CExeModule::FreeSchema418()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
		struct{
			unsigned __int32 version:12;
			unsigned __int32 model:12;
			unsigned __int32 mfgctry:8;
		};
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Румынии
	case 0x0E:
		switch(model){
		//case 0x200://0E200001 - Молдавия
		//case 0x712://0E712001 - Молдавия
		case 0xF37://0EF37001
			if((version&0xFFC)==0)
				break;
			return false;
		case 0x411://0E411001
			switch(version&0xFFC){
			case 0:
			case 0xb00:
			case 0xe00:
				break;
			default:
				return false; //0E411A01 - Молдавия
			}
			break;
		case 0x412://0E412001
			switch(version&0xFFC){
			case 0xa00:
				break;
			default:
				return false; //0E412001 и 0E412B01 - Молдавия
			}
			break;
		default:
			return false;
		}
		break;
	case 0x9E:
		switch(ui8[2]){
		case 0x44://9E440000-3
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xE4:
		switch(ui8[2]){
		case 0x01://E4010002 E4010001
			if(ui16<3) // E4010003 - Молдавия
				break;
			//if((ui16&0xFFFC)==0)
			//	break;
		default:
			return false;
		}
		break;
	case 0xF4:
		switch(ui8[2]){
		case 0x00://F4000002 F4000001
		case 0x20://F4200002 F4200001
		case 0x32://F4320001  - фискальный принтер
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
// Армен Харатян сначала попросил сделать бесплатную работу, а потом попросил вернуть всё в зад
#if 0
	case 0xF5:
		switch(ui8[2]){
		case 0x00://F5000010 F5000011 F5000021
			switch(ui16){
			case 0x0010:
			case 0x0011:
			case 0x0021:
				break;
			default:
				return false;
			}
			break;
		case 0x60://F5600001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
#endif
	case 0xF7:
		switch(ui8[2]){
		case 0x43://F7430002 F7430001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::FreeKenya()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Кении
	case 0xC4:
		switch(ui8[2]){
		case 0x21://C4210001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xC7:
		switch(ui8[2]){
		case 0x43://C7430001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::FreeHungary()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Венгрии
	case 0xA4:
		switch(ui8[2]){
		case 0x00://A4000001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xA7:
		switch(ui8[2]){
		case 0x43://A7430001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xA8:
		switch(ui8[2]){
		case 0x00://A8000001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::FreeMalta()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
		struct{
			unsigned __int32 version:12;
			unsigned __int32 model:12;
			unsigned __int32 mfgctry:8;
		};
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Мальты
	case 0x0a:
		switch(model){
		case 0x200://0a200771,0a200781
			switch(version&0xFFC){
			case 0x770:
			case 0x780:
				break;
			default:
				return false;
			}
		default:
			return false;
		}
		break;
	case 0xA4:
		switch(ui8[2]){
		case 0x01://A4010001
		case 0x32://A4320001
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	case 0xA7:
		switch(ui8[2]){
		case 0x43://A7430002
			if((ui16&0xFFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::FreeGreece()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
		struct{
			unsigned __int32 version:12;
			unsigned __int32 model:12;
			unsigned __int32 mfgctry:8;
		};
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для Греции
	case 0x01:
		switch(model){
		case 0x712://01712001
		case 0x900://01900001
		case 0x417://01417001
		case 0x419://01419000-3
			if((version&0xFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::FreeRussia()
{
	union{
		unsigned __int32 id;
		unsigned __int16 ui16;
		unsigned __int8 ui8[4];
		struct{
			unsigned __int32 version:12;
			unsigned __int32 model:12;
			unsigned __int32 mfgctry:8;
		};
	};
	int size=sizeof(id);
	sh->get_device_id(&id,&size);
	switch(ui8[3]){
	//номера схем для России
	case 0x98:
		switch(model){
		case 0x510://98510001
			if((version&0xFFC)==0)
				break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

__forceinline bool CExeModule::Security()
{
	using namespace std;
	if(FreeSchema402()||FreeSchema418()||FreeKenya()||FreeHungary()||FreeMalta()||FreeGreece()||FreeRussia())
		return true;
	hcdb::select selft(string("FTax"));
	auto_ptr<rowset> rsft(new rowset(sh));
	selft.result_rows=rsft.get();
	ccerr::exec(sess,&selft);
	rsft->move(0,rows::first);
	if(!rsft->eof()||Author){
		hcdb::select sel(string("Time"));
		auto_ptr<rowset> rs(new rowset(sh));
		sel.result_rows=rs.get();
		ccerr::exec(sess,&sel);
		rs->move(0,rows::first);
		_variant_t date=(*rs)["Date"];
#ifdef HARD_LOCK
		if(date.date>(365.0*101.0-65.0))//это примерно 1 октября
			CloseSession();
#else
		if(!ScanKeysFile(sess,date.date)){
			CloseSession();
			return false;
		}
#endif
	}
	return true;
}

DWORD xvFormatMessage(LPCTSTR&lpMsgBuf,HMODULE lpSource,DWORD dwMessageId,va_list vl)
{
	return FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_HMODULE,
			lpSource,
			dwMessageId,
			0,
			(LPTSTR)&lpMsgBuf,
			0,
			&vl);
}

DWORD xvFormatMessage(LPCTSTR&lpMsgBuf,LPCTSTR lpSource,DWORD dwMessageId,va_list vl)
{
	return FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING,
			lpSource,
			dwMessageId,
			0,
			(LPTSTR)&lpMsgBuf,
			0,
			&vl);
}

DWORD xFormatMessage(_bstr_t*res,LPCTSTR lpSource,...)
{
	va_list vl;
	va_start(vl,lpSource);
	LPVOID lpMsgBuf;
	DWORD hr=FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING,
		lpSource,
		0,
		0,
		(LPTSTR) &lpMsgBuf,
		0,
		&vl);
	va_end(vl);
	if(hr){
		*res=(LPCTSTR)lpMsgBuf;
		LocalFree( lpMsgBuf );
	}
	return hr;
}

size_t RTrim(_bstr_t const&cn,OLECHAR*const buf)
{
	OLECHAR const*src=cn;
	OLECHAR*dst=buf;
	OLECHAR*endcandidat=dst;
repeat:
	switch(*dst++=*src++){
	case 0:
		break;
	case L' ':
		goto repeat;
	default:
		endcandidat=dst;
		goto repeat;
	}
	*endcandidat=0;
	return endcandidat-buf;
}

#include <hctlb.h>

HRESULT CExeModule::Register(BSTR Machine, long Port, long LogNum, long User, long Password)
{
	using namespace std;
	CloseSession();
	USES_CONVERSION;
	string mach=Machine?string(W2A(Machine)):"";
	if(Port!=params.Port||LogNum!=params.LogNum||User!=params.UName||Password!=params.Password||mach!=params.Machine){
		device*dev=params.getdev(Port,LogNum,mach);
		try{
			ModifyTrayIcon(mtiYellow);
			struct guard
			{
				guard(){Waiter=true;}
				~guard(){Waiter=false;}
			}g;
			sess=params.getsess(dev,User,Password);
			sh=sess->get_shema();
		}
		catch(...){
			params.Port=Port;
			params.LogNum=LogNum;
			params.Machine=mach;
			params.UName=User;
			params.Password=Password;
			sess=reinterpret_cast<session*>(1L);
			sh=NULL;
			CloseSession();
			throw;
		}
		if(sess){
			Online=true;
			ModifyTrayIcon(mtiGreen);
			sess->set_handler(this);
			//Настройка параметров алгоритма защиты
#ifndef HARD_LOCK
			ap.swap_nibbles_mode=1;
			ap.xor_cycles=5;
			ap.swapinbuf=swapinbufVSU;
			ap.swapinbuf_cycles=5;
#endif
			//
			params.Port=Port;
			params.LogNum=LogNum;
			params.Machine=mach;
			params.UName=User;
			params.Password=Password;

			//сохраняем серийный номер в локальных переменных на тот случай если нет ключа
			long num;
			wchar_t let[3];
			{
				_idlSerNum const&sn=sess->ser_num();
				num=sn.number;
				let[0]=sh->cp.from_cash(reinterpret_cast<char const*>(&sn.letters)[0]);
				let[1]=sh->cp.from_cash(reinterpret_cast<char const*>(&sn.letters)[1]);
				let[2]=0;
				//сохраняем число знаков после запятой в деньгах и в количестве
				PROP::pcrec prit;

				ScaleAmount=3;
				prit=sh->prop.prop(diScaleAmount);
				if(sh->prop.valid(prit))
					ScaleAmount=prit->value;

				ScaleMoney=2;
				prit=sh->prop.prop(diScaleMoney);
				if(sh->prop.valid(prit))
					ScaleMoney=prit->value;

				ScalePrc=2;
				prit=sh->prop.prop(diScalePrc);
				if(sh->prop.valid(prit))
					ScalePrc=prit->value;
			}

			//Читаем и сохраняем имя оператора
			if(User!=0){
				hcdb::select sel(string("Oper"));
				rowset Oper(sh);
				sel.result_rows=&Oper;
				sel.i_fields.push_back(new field_value("ID",User));
				try{
					ccerr::exec(sess,&sel);
					_bstr_t cn=Oper["Name"];
					OLECHAR*const buf=(OLECHAR*)_alloca((cn.length()+1)*sizeof(OLECHAR));
					RTrim(cn,buf);
					CashierName=buf;
				}
				catch(...){
				}
			}

			if(!Security()){
				//Здесь нельзя пользоваться сессией, так как она закрывается внутри Security если Security возвращает ложь
#ifdef HARD_LOCK
				MessageBox(NULL,_T("Демонстрационный период для данной программы закончился"),_T("Система защиты от несанкционированного использования"),MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
#else
				int const bufsize=256;
				TCHAR buf[bufsize];
				LoadString(m_hInstResource,IDS_EXPIRED,buf,bufsize-1);
				_bstr_t title;
				xFormatMessage(&title,TEXT("%1!ls! %2!lu!"),let,num);
				MessageBox(NULL,buf,title,MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
#endif
			}
		}
	}
	if(sess){
		extern unsigned long globsernum;
		globsernum=sess->ser_num().number;
	}
	return sess?S_OK:E_FAIL;
}

class fileitf
{
public:
	virtual void getline(char*,int)=0;
	virtual void updateline(char*,int);
	virtual operator bool()=0;
	virtual ~fileitf();
};

void fileitf::updateline(char*,int){}
fileitf::~fileitf(){}

class file:public fileitf
{
	std::ifstream ifs;
public:
	file(BSTR);
	virtual void getline(char*,int);
	virtual operator bool();
	virtual ~file();
};

file::file(BSTR File)
{
	USES_CONVERSION;
	ifs.open(W2A(File));
}

void file::getline(char*buf,int bufsize)
{
	ifs.getline(buf,bufsize);
}

file::operator bool()
{
	return !!ifs;
}

file::~file(){}


class rwfile:public fileitf
{
	std::streampos getp;
	std::streampos putp;
	std::fstream fs;
public:
	rwfile(BSTR);
	virtual void getline(char*,int);
	virtual void updateline(char*,int);
	virtual operator bool();
	virtual ~rwfile();
};

rwfile::rwfile(BSTR File):getp(0),putp(0)
{
	USES_CONVERSION;
	fs.open(W2A(File));
}

void rwfile::getline(char*buf,int bufsize)
{
	putp=getp;
	fs.seekg(getp);
	fs.getline(buf,bufsize);
	getp=fs.tellg();
}

void rwfile::updateline(char*buf,int bufsize)
{
	fs.seekp(putp);
	fs.write(buf,bufsize);
}

rwfile::operator bool()
{
	return !!fs;
}

rwfile::~rwfile(){}

////////////////////////////////////
//эта версия функции отличается от той, котороая используется в проекте plumu
void csFromRegistry(hkey&base,LPCTSTR lpValueName,_bstr_t&name,_bstr_t&cs)
{
	_variant_t v_erst;
	try{
		v_erst=base[lpValueName];
	}
	catch(hkey::regerror&){
		//ожидаемое исключение
	}
	if(v_erst.vt!=VT_BSTR){
		v_erst=name;
	}
	//if(v_erst.vt==VT_BSTR){
		_bstr_t bstr=v_erst;
		wchar_t *wcs=bstr;
		if(wcschr(wcs,L'=')){
			//connection string
			int charsread=0;
			swscanf(wcs,L" vsu = %n",&charsread);
			if(charsread>0){
				//имя указано
				wchar_t *semicolon=wcschr(wcs+charsread,L';');
				if(semicolon){
					*semicolon=0;
					name=wcs+charsread;
					*semicolon=L';';
					cs=semicolon+1;
				}
			}
			else{
				//имя не указано
				cs=wcs;
			}
		}
		else{
			//file name
			wchar_t*namepart=wcs;
			wchar_t*bslash=wcsrchr(wcs,L'\\');
			if(bslash){
				namepart=bslash+1;
				*bslash=0;
			}
			wchar_t*ext=wcsrchr(namepart,L'.');
			if(ext){
				*ext=0;
				name=namepart;
				*ext=L'.';
			}
			else
				name=namepart;
			USES_CONVERSION;
			DWORD nBufferLength;
			dynarray<_TCHAR> curdir(nBufferLength=GetCurrentDirectory(0,NULL));
			if(!bslash){
				GetCurrentDirectory(nBufferLength,curdir);
				wcs=T2W((_TCHAR*)curdir);
			}
			static wchar_t const*const defconnstr=L"Driver={Microsoft Visual FoxPro Driver};SourceDB=%s;SourceType=DBF;";
			dynarray<wchar_t> connstr(wcslen(defconnstr)+wcslen(wcs)+1);
			swprintf(connstr,defconnstr,wcs);
			cs=connstr;
			if(bslash)
				*bslash=L'\\';
		}
	//}
/*	else{
		//вставляем текущую директорию
		USES_CONVERSION;
		DWORD nBufferLength;
		dynarray<_TCHAR> curdir(nBufferLength=GetCurrentDirectory(0,NULL));
		GetCurrentDirectory(nBufferLength,curdir);
		wchar_t const*wcs=T2W((_TCHAR*)curdir);
		dynarray<wchar_t> connstr(wcslen(cs)+wcslen(wcs)+1);
		swprintf(connstr,cs,wcs);
		cs=connstr;
	}*/
}
////////////////////////////////////

class dbf:public fileitf
{
	_bstr_t base_cs;
	_bstr_t base_name;
	_RecordsetPtr rst;
	int lines;
public:
	dbf(BSTR);
	virtual void getline(char*,int);
	virtual operator bool();
	virtual ~dbf();
};

dbf::dbf(BSTR File):
	base_cs(L"Driver={Microsoft Visual FoxPro Driver};SourceDB=%s;SourceType=DBF;"),
	base_name(L"cmdbase"),
	lines(0)
{
	base_name=File;
	csFromRegistry(hvsu,_T("base_cs"),base_name,base_cs);
	rst.CreateInstance(L"ADODB.Recordset");
	_ConnectionPtr conn(L"ADODB.Connection");
	conn->Open(base_cs,L"",L"",0);
	rst->Open(base_name,(IDispatch*)conn,adOpenDynamic,adLockOptimistic,adCmdTable);
	extern _RecordsetPtr globrst;
	globrst=rst;
}

void dbf::getline(char*buf,int bufsize)
{
	if(lines++)
		rst->MoveNext();
	if(!(rst->GetEOF()==VARIANT_FALSE)){
		*buf=0;
		return;
	}
	_bstr_t res;
	res=rst->Fields->GetItem(0L)->Value;
	res+=L";";
	res+=_bstr_t(rst->Fields->GetItem(1L)->Value);
	USES_CONVERSION;
	strncpy(buf,W2A(res),bufsize);
	buf[bufsize-1]=0;
	rst->Fields->GetItem(2L)->Value=L"OK";
}

dbf::operator bool()
{
	if(rst==NULL)
		return false;
	return rst->GetEOF()==VARIANT_FALSE;
}

dbf::~dbf(){}

bool GetDouble(char*&cl,_variant_t&dbl){
	size_t len;
	char*sc=strchr(cl,';');
	if(sc){
		*sc=0;
		len=sc-cl;
	}
	else
		len=strlen(cl);
	{
		char*d=reinterpret_cast<char*>(_alloca(len+1));
		int const bufsize=8;
		TCHAR decsep[bufsize];
		char*todbl=cl;
		if(GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,decsep,bufsize)){
			char from=0;
			char to=0;
			if(decsep[0]==TEXT('.')){
				from=',';//comma
				to='.';//dot
			}
			else if(decsep[0]==TEXT(',')){
				from='.';//dot
				to=',';//comma
			}
			if(from){
				todbl=d;
				char const*s=cl;
				do{
					if(*s==from)
						*d=to;
					else
						*d=*s;
					s++;
				}
				while(*d++);
			}
		}
		dbl=todbl;
	}
	try{
		dbl.ChangeType(VT_R8);
	}
	catch(_com_error&){
		if(sc)
			*sc=';';
		return false;
	}
	if(sc){
		*sc=';';
		cl=sc+1;
	}
	else{
		cl+=len;
	}
	return true;
}

DWORD globerr=0;
unsigned string_in_process=0;
bool isExcludeCharacterDetected = false;

std::ostream&operator<<(std::ostream&out,getmsg const&gm){
	return out<<gm.lpMsgBuf;
}

#if 0

class cemsg
{
	HINSTANCE dll;
public:
	cemsg();
	~cemsg();
	string operator()(DWORD ecode,LPCTSTR defmsg);
};

cemsg::cemsg():dll(LoadLibrary(_T("vsuerr.dll")))
{
}

cemsg::~cemsg()
{
	if(dll)
		FreeLibrary(dll);
}

string cemsg::operator()(DWORD ecode,LPCTSTR defmsg)
{
	globerr=ecode;
	if(dll){
		LPTSTR tstr=NULL;
		DWORD dw=FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_HMODULE,
			dll,
			ecode,
			0,
			(LPTSTR)&tstr,
			0,
			NULL);
		if(!dw){
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR)&tstr,
				0,
				NULL);
		}
		string ret(tstr);
		if(tstr)
			LocalFree(tstr);
		return ret;
	}
	else
		return defmsg;
}

cemsg emsg;

#endif

void CExeModule::LineFormat0(LineContext0*ctx,char *const line,bool&error,unsigned&securcnt)
{
	char*cl=line;
	while(*cl==' '||*cl=='\t')cl++;
	if(cl[0]==0||cl[0]==';')
		throw cycleContinue();
	if(cl[1]!=';'){
		out<<emsg(st_syntax_error,"Syntax error in line: ")<<line<<endl;
		error=true;
		throw cycleContinue();
	}
	cl+=2;
	int charsread;
	unsigned long ExgState=0;
	switch(cl[-2]){
	case 0:
		securcnt++;
		break;
	case 'X':// обмен (BegExchange)
		ExgState=1;
	case 'S'://Продажа
		securcnt++;
		if(sh->tabl.valid(sh->tabl.table("BegExchange")))
			CallProc("BegExchange",ExgState);
		else if(ExgState==1)
			break;
		{
			unsigned __int64 Code;
			char*namebeg;
			char*nameend;
			_variant_t Cost;
			_variant_t Amount;
			unsigned long Dept,Group,Tax;
#ifdef ALL_FLAGS
			unsigned long Flags;
#else
			unsigned long Fract,Free;
#endif
#ifdef STRICT_INTEGRITY
			bool bFlg=true,bTax=true,bName=true;
#endif
			char*commbeg;
			char*commend;
			bool bComm=false;
			char*unitbeg;
			char*unitend;
			bool bUnit=false;

			if(1!=sscanf(cl," %I64u ; %n",&Code,&charsread)){
				out<<emsg(st_invalid_plu_code,"Invalid PLU code: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;
			
			namebeg=cl;
			nameend=strchr(cl,';');
			bName=namebeg!=nameend;
			if(!nameend){
				out<<emsg(st_invalid_plu_name,"Invalid command: ")<<line<<endl;
				error=true;
				break;
			}
			cl=nameend+1;

			if(cl[0]==';'){
				Cost=0.0;
				charsread=1;
				cl+=charsread;
			}
			else if(!GetDouble(cl,Cost)){
				out<<emsg(st_invalid_plu_price,"Invalid price of selling: ")<<line<<endl;
				error=true;
				break;
			}

			if(cl[0]==';'){
				Amount=0.0;
				charsread=1;
				cl+=charsread;
			}
			else if(!GetDouble(cl,Amount)){
				out<<emsg(st_invalid_plu_amount,"Invalid amount of selling: ")<<line<<endl;
				error=true;
				break;
			}

			if(cl[0]==';'){
				Dept=0;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Dept,&charsread)){
				out<<emsg(st_invalid_plu_dept,"Invalid dept of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

			if(cl[0]==';'){
				Group=0;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Group,&charsread)){
				out<<emsg(st_invalid_plu_group,"Invalid group of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

			if(cl[0]==';'){
				Tax=0;
				charsread=1;
				bTax=false;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Tax,&charsread)){
				out<<emsg(st_invalid_plu_tax,"Invalid tax of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

#ifdef OLD_FORMAT
			if(cl[0]==';'){
				Fract=0;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Fract,&charsread)){
				out<<emsg(st_invalid_plu_fract,"Invalid fract of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

			if(cl[0]==';'){
				Free=0;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Free,&charsread)){
				out<<emsg(st_invalid_plu_free,"Invalid free of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;
#elif defined(ALL_FLAGS)
			//Приоритеты использования флагов
			//низший: 3 (если в схеме не описаны флаги)
			//низкий: из схемы
			//высокий: из реестра
			//высший: указанный во входном файле
			if(cl[0]==';'){
				if(DefaultFlags)
					Flags=DefaultFlags;
				else{
					hcdb::PROP::pcrec MutablePrice=sh->prop.prop("MutablePrice");
					hcdb::PROP::pcrec Fractional=sh->prop.prop("Fractional");
					if(sh->prop.valid(MutablePrice)&&sh->prop.valid(Fractional))
						Flags=long(MutablePrice->value)|long(Fractional->value);
					else
						Flags=2|1;//Fract,Free in most ecrs
				}
				charsread=1;
				bFlg=false;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Flags,&charsread)){
				out<<emsg(st_invalid_plu_flags,"Invalid flags of selling: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;
#else
			Fract=1;
			Free=1;
#endif

			commbeg=cl;
			commend=strchr(cl,';');
			if(commend)
				cl=commend+1;
			else{
				commend=commbeg+strlen(commbeg);
				cl=commend;
			}
			bComm=commbeg<commend;

			unitbeg=cl;
			unitend=strchr(cl,';');
			if(unitend)
				cl=unitend+1;
			else{
				unitend=unitbeg+strlen(unitbeg);
				cl=unitend;
			}
			bUnit=unitbeg<unitend;

#if defined(FULLPROD)
			if((bComm||bUnit)&&sh->tabl.valid(sh->tabl.table("FullNameProd"))){
				CallProc("FullNameProd",
					Code,
					Cost,
					Amount,
					Dept,
					Group,
					Tax,
					std::string(namebeg,nameend),
					std::string(commbeg,commend),
					std::string(unitbeg,unitend));
			}
			else if(sh->tabl.valid(sh->tabl.table("FullProd"))){
				CallProc("FullProd",Code,Cost,Amount,Dept,Group,Tax,std::string(namebeg,nameend));
			}
			else{
#endif
				unsigned ecrerr;
#ifdef STRICT_INTEGRITY
				{
					hcdb::select sel(std::string("PLU"));
					std::auto_ptr<hcdb::rowset> PLU(new rowset(sh));
					sel.result_rows=PLU.get();
					sel.i_fields.push_back(new field_value("Code",double(__int64(Code))));
					try{
						ccerr::exec(sess,&sel);
					}
					catch(ccerr::cmderror&e){
						ecrerr=e.ecrerr;
						switch(ecrerr){
						case 0x40://key not found
							break;
						default:
							throw;
						}
					}
					PLU->move(0,rows::first);
					if(!PLU->eof()){//если товар есть, то пытаемся его проверить, иначе он будет запрограммирован ниже
						update upd(std::string("PLU"));
						upd.i_fields.push_back(new field_value("Code",double(__int64(Code))));
						bool equal=true;
						if(bFlg){
							unsigned long Flags_=long(_variant_t((*PLU)["Flg"]));
							if(Flags_!=Flags){
								equal=false;
								upd.fields.push_back(new field_value("Flg",long(Flags)));
							}
						}
						if(bTax){
							unsigned long Tax_=long(_variant_t((*PLU)["Tax"]));
							if(Tax_!=Tax){
								equal=false;
								upd.fields.push_back(new field_value("Tax",long(Tax)));
							}
						}
						if(bName){
							_variant_t var((*PLU)["Name"]);
							_variant_t v2(std::string(namebeg,nameend).c_str());
							assert(v2.vt==VT_BSTR);
							size_t st=wcslen(v2.bstrVal);
							dynarray<char> buf(st);
							sh->cp.to_cash_str(buf,st,v2.bstrVal);
							sh->cp.from_cash_str(buf,st,v2.bstrVal);
							size_t varsz=wcslen(var.bstrVal);
							bool sequal;
							if(st<varsz){
								//добить пробелами v2 до varsz символов и сравнить
								dynarray<wchar_t> ss(varsz);
								wcsncpy(ss,v2.bstrVal,st);
								while(st<varsz)
									static_cast<wchar_t*>(ss)[st++]=L' ';
								sequal=!wcsncmp(var.bstrVal,ss,varsz);
							}
							else{
								//сравнить столько символов, сколько прислала касса
								sequal=!wcsncmp(var.bstrVal,v2.bstrVal,varsz);
							}
							if(!sequal){
								equal=false;
								upd.fields.push_back(new field_value("Name",std::string(namebeg,nameend).c_str()));
							}
						}
						if(!equal){
							ccerr::exec(sess,&upd);
						}
					}
				}
#endif
				
				bool realDept=false;
				bool realGroup=false;
retryProd:
				ve.str("");
				ecrerr=0;
				try{
					//#ifdef DIMABUG2
					CallProc("Prod",Code,Cost,Amount,realDept?Dept:unsigned long(0),realGroup?Group:unsigned long(0));
					//#else
					//						CallProc("Prod",Code,Cost,Amount,Dept,Group);
					//#endif
				}
				catch(ccerr::cmderror&e){
					ecrerr=e.ecrerr;
					switch(ecrerr){
					case 0x03://не указан отдел
						if(Dept)
							realDept=true;
						else
							throw;
						break;
					case 0x04://не указана группа
						if(Group)
							realGroup=true;
						else
							throw;
						break;
					case 0xee:
						break;
					default:
						throw;
					}
					out<<e;
				}
				switch(ecrerr){
				case 0:
					break;
				case 0x03:
					goto retryProd;
				case 0x04:
					goto retryProd;
				case 0xee:
					{
						//сформировать команду вставки
						insert ins(std::string("PLU"));
						ins.fields.push_back(new field_value("Code",double(__int64(Code))));
						ins.fields.push_back(new field_value("Name",std::string(namebeg,nameend).c_str()));
#ifdef ALL_FLAGS
						ins.fields.push_back(new field_value("Cen",(Flags&1)?0.0:Cost));
#else
						ins.fields.push_back(new field_value("Cen",Free?0.0:Cost));
#endif
						ins.fields.push_back(new field_value("Dep",long(Dept)));
						ins.fields.push_back(new field_value("Grp",long(Group)));
						ins.fields.push_back(new field_value("Tax",long(Tax)));
						ins.fields.push_back(new field_value("Kol",double(0.0)));
#ifdef ALL_FLAGS
						ins.fields.push_back(new field_value("Flg",long(Flags)));
#else
						ins.fields.push_back(new field_value("Flg",long((Fract?2:0)|(Free?1:0))));
#endif
						//запустить команду вставки
						ccerr::exec(sess,&ins);//в случае исключений я не знаю как восстанавливаться, поэтому пусть исключения вылетают дальше
						//если вставка прошла успешно, то очистить последствия исключения и повторить попытку продать товар
						goto retryProd;
					}
					break;
				default:
					assert(false);
				}
#if defined(FULLPROD)
			}
#endif
		}
		break;
	case 'P'://Комментарий
		securcnt++;
		{
			char* end=strchr(cl,';');
			if(end){
				CallProc("TextComment",std::string(cl,end));
			}
			else{
				out<<emsg(st_invalid_comment,"Invalid comment command: ")<<line<<endl;
				error=true;
			}
		}
		break;
	case 'C'://Альтернативный Комментарий
		securcnt++;
		{
			char* end=strchr(cl,';');
			if(end){
				CallProc("PrintComment",std::string(cl,end));
			}
			else{
				out<<emsg(st_invalid_comment,"Invalid comment command: ")<<line<<endl;
				error=true;
			}
		}
		break;
	case 'F'://Печать на бланке
		securcnt++;
		{
			char* end=strchr(cl,';');
			if(end){
				CallProc("PrintOnForm",std::string(cl,end));
			}
			else{
				out<<emsg(st_invalid_comment,"Invalid comment command: ")<<line<<endl;
				error=true;
			}
		}
		break;
	case 'W'://Выплата
		securcnt++;
		{
			unsigned long Wid;
			_variant_t Sum;

			if(cl[0]==';'){
				Wid=1;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Wid,&charsread)){
				out<<emsg(st_invalid_payment_kind,"Invalid kind of payment: ")<<line<<endl;
				error=true;
				break;
			}
			Wid--;
			cl+=charsread;

			if(cl[0]==';'){
				Sum=0.0;
				charsread=1;
			}
			else if(!GetDouble(cl,Sum)){
				out<<emsg(st_invalid_payment_sum,"Invalid sum of payment: ")<<line<<endl;
				error=true;
				break;
			}

			CallProc("PayOff",Wid,Sum);
		}
		break;
	case 'T'://Оплата
		securcnt++;
		{
			unsigned long Wid;
			_variant_t Sum;
			unsigned long Code;

			if(cl[0]==';'){
				Wid=1;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Wid,&charsread)){
				out<<emsg(st_invalid_payment_kind,"Invalid kind of payment: ")<<line<<endl;
				error=true;
				break;
			}
			Wid--;
			cl+=charsread;

			if(cl[0]==';'){
				Sum=0.0;
				charsread=1;
				cl+=charsread;
			}
			else if(!GetDouble(cl,Sum)){
				out<<emsg(st_invalid_payment_sum,"Invalid sum of payment: ")<<line<<endl;
				error=true;
				break;
			}

			if(cl[0]==';'){
				Code=0;
				charsread=1;
			}
			else if(1!=sscanf(cl," %lu ; %n",&Code,&charsread)){
				out<<emsg(st_invalid_payment_code,"Invalid custom code: ")<<line<<endl;
				error=true;
				break;
			}

			CallProc("Oplata",Wid,Sum,Code);
		}
		break;
	case 'D'://Скидки
		securcnt++;
		{
			unsigned long Wid;
			_variant_t Sum=0.0;
			_variant_t Prc=0.0;

			if(1!=sscanf(cl," %lu ; %n",&Wid,&charsread)){
				out<<emsg(st_invalid_discount_kind,"Invalid kind of discount: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

			if(cl[0]==';'){
				Sum=0.0;
			}
			else if(!GetDouble(cl,Sum)){
				out<<emsg(st_invalid_discount_sum,"Invalid sum of discount: ")<<line<<endl;
				error=true;
				break;
			}

			if(Wid&2){
				Prc=Sum;
				Sum=0.0;
			}

			CallProc("NacSkd",Wid,Sum,Prc);
		}
		break;
	case 'I'://Внос/вынос денег
		securcnt++;
		{
			unsigned long Wid;
			_variant_t Sum=0.0;

			if(1!=sscanf(cl," %lu ; %n",&Wid,&charsread)){
				out<<emsg(st_invalid_inout_kind,"Invalid kind of deposit/withdrawal: ")<<line<<endl;
				error=true;
				break;
			}
			Wid--;
			cl+=charsread;

			if(!GetDouble(cl,Sum)){
				out<<emsg(st_invalid_inout_sum,"Invalid sum of deposit/withdrawal: ")<<line<<endl;
				error=true;
				break;
			}

			CallProc("InOut",Wid,Sum);
		}
		break;
	case 'B'://бинарный комментарий
		securcnt++;
		{
			unsigned long Id;

			int ssres=sscanf(cl," %lu ; %n",&Id,&charsread);
			if(1!=ssres||(ssres==1&&Id>255)){
				out<<emsg(st_invalid_bin_id,"Invalid binary comment identifier: ")<<line<<endl;
				error=true;
				break;
			}
			cl+=charsread;

			char*end=strchr(cl,';');
			if(end){
				CallProc("BinComment",Id,std::string(cl,end));
			}
			else{
				out<<emsg(st_invalid_bin_data,"Invalid binary comment data: ")<<line<<endl;
				error=true;
			}
		}
		break;
	case 'V'://отмена
		securcnt++;
		{
			unsigned __int64 Code=0;

			if(cl[0]==';'){
				charsread=1;
			}
			else if(1!=sscanf(cl," %I64u ; %n",&Code,&charsread)){
					out<<emsg(st_invalid_plu_code,"Invalid PLU code: ")<<line<<endl;
					error=true;
					break;
			}
			cl+=charsread;

			if(Code==0){
				//VoidLast
				CallProc("VoidLast");
			}
			else{
				//VoidProd
				CallProc("VoidProd",Code);
			}
		}
		break;
	case 'N'://BegVatFact
		securcnt++;
		{
			unsigned __int64 Code=0;
			if(cl[0]==';'){
				charsread=1;
			}
			else if(1!=sscanf(cl," %I64u ; %n",&Code,&charsread)){
					out<<emsg(st_invalid_vatfact_code,"Invalid VAT Facture code: ")<<line<<endl;
					error=true;
					break;
			}
			cl+=charsread;

			if(!ProcExists("BegVatFact")){
				out<<emsg(st_no_BegVatFact,"Cash register does not support facture printing: ")<<line<<endl;
				error=true;
				break;
			}

			procedure proc(std::string("BegVatFact"));
			proc<<Code;
			exec(&proc);
		}
		break;
	case 'R'://BegReturn
		securcnt++;
		{
			if(!ProcExists("BegReturn")){
				out<<emsg(st_no_BegReturn,"Cash register does not support return receipts: ")<<line<<endl;
				error=true;
				break;
			}

			procedure proc(std::string("BegReturn"));
			exec(&proc);
		}
		break;
	default:
		out<<emsg(st_invalid_cmdid,"Unknown command identifier in line: ")<<line<<endl;
		error=true;
	}
}

//isl_prn.pdf
HRESULT CExeModule::LineFormat1(LineContext1*ctx,char *const line,unsigned&securcnt)
{
	DWORD MessageId=VSU_SyntaxError;
	int wrongpos=1;
	int assigned=EOF;
	switch(line[0]){
	case 0:
		throw cycleContinue();
		break;
	case 'R':
		securcnt++;
		{
			int bPrice=0;
			double Price=0.0;
			int ePrice=0;
			int bQty=0;
			double Qty=0.0;
			int eQty=0;
			int bTax=0;
			long Tax;
			int eTax=0;
			int bCode=0;
			unsigned __int64 Code;
			int eCode=0;
			char Space;
			int eSpace=0;
			char Name[65];
			int eName=0;

			assigned=sscanf(line,"R"
				" %n%10lf%n"
				" %n%10lf%n"
				" %n%1lu%n"
				" %n%6I64u%n"
				"%c%n"
				"%64c%n",
				&bPrice,&Price,&ePrice,
				&bQty,&Qty,&eQty,
				&bTax,&Tax,&eTax,
				&bCode,&Code,&eCode,
				&Space,&eSpace,
				Name,&eName);
			switch(assigned){
			case 0:
				MessageId=VSU_PriceNotParsed;
				wrongpos=bPrice;
				break;
			case 1:
				MessageId=VSU_QtyNotParsed;
				wrongpos=bQty;
				break;
			case 2:
				MessageId=VSU_TaxNotParsed;
				wrongpos=bTax;
				break;
			case 3:
				MessageId=VSU_CodeNotParsed;
				wrongpos=bCode;
				break;
			case 4:
				MessageId=VSU_SpaceNotParsed;
				wrongpos=eCode;
				break;
			case 5:
				MessageId=VSU_NameNotParsed;
				wrongpos=eSpace;
				break;
			case 6:
				//все поля присвоены
				MessageId=S_OK;
				Name[eName-eSpace]=0;
#if 0
				if(ePrice!=11){
					out<<emsg(st_invalid_plu_price,"Syntax error in price field.")<<endl
						<<"Parsing failed at position "<<ePrice<<" in line:"<<endl
						<<line<<endl;
					error=true;
					break;
				}
				if(eQty!=22){
					out<<emsg(st_invalid_plu_amount,"Wrong position of quantity: ")<<line<<endl;
					error=true;
					break;
				}
				if(eTax!=24){
					out<<emsg(st_invalid_plu_tax,"Wrong position of tax: ")<<line<<endl;
					error=true;
					break;
				}
				//if(Tax)
				if(eCode!=31){
					out<<emsg(st_invalid_plu_code,"Wrong position of code: ")<<line<<endl;
					error=true;
					break;
				}
				if(Space!=' '){
				}
				if(bName!=32){
					out<<emsg(st_invalid_plu_name,"Wrong position of name: ")<<line<<endl;
					error=true;
					break;
				}
#endif
				//CallProc("FullProd",Code,Price,Qty,1LU,1LU,Tax,string(Name));
				procedure proc(std::string("FullProd"));
				proc<<Code<<Price<<Qty<<1<<1<<Tax<<Name;
				exec(&proc);
				break;
			}
		}
		break;
	case 'S':
		securcnt++;
		{
			int bPrice=0;
			double Price=0.0;
			int ePrice=0;
			int bQty=0;
			double Qty=0.0;
			int eQty=0;
			int bTax=0;
			long Tax;
			int eTax=0;
			int bDept=0;
			long Dept;
			int eDept=0;
			char Space;
			int eSpace=0;
			char Name[65];
			int eName=0;

			assigned=sscanf(line,"S"
				" %n%10lf%n"
				" %n%10lf%n"
				" %n%1lu%n"
				" %n%1lu%n"
				"%c%n"
				"%64c%n",
				&bPrice,&Price,&ePrice,
				&bQty,&Qty,&eQty,
				&bTax,&Tax,&eTax,
				&bDept,&Dept,&eDept,
				&Space,&eSpace,
				Name,&eName);
			switch(assigned){
			case 0:
				MessageId=VSU_PriceNotParsed;
				wrongpos=bPrice;
				break;
			case 1:
				MessageId=VSU_QtyNotParsed;
				wrongpos=bQty;
				break;
			case 2:
				MessageId=VSU_TaxNotParsed;
				wrongpos=bTax;
				break;
			case 3:
				MessageId=VSU_DeptNotParsed;
				wrongpos=bDept;
				break;
			case 4:
				MessageId=VSU_SpaceNotParsed;
				wrongpos=eDept;
				break;
			case 5:
				MessageId=VSU_NameNotParsed;
				wrongpos=eSpace;
				break;
			case 6:
				//все поля присвоены
				MessageId=S_OK;
				Name[eName-eSpace]=0;
				{
					procedure proc(std::string("TextComment"));
					proc<<Name;
					exec(&proc);
				}
				{
					_variant_t Code;
					Code.vt=VT_CY;
					Code.cyVal.int64=140737488355327-Dept;
					Code.cyVal.int64*=10000;
					try{
						TCHAR value[16];
						wsprintf(value,TEXT("%lu"),Dept);
						Code=hkey(hvsu,TEXT("dept"))[value];
					}
					catch(hkey::regerror&){}
					procedure proc(std::string("FullProd"));
					proc<<Code<<Price<<Qty<<1<<1<<Tax<<""/*ReadValueByID("Dep","Name",Dept)*/;
					exec(&proc);
				}
				break;
			}
		}
		break;
	case 'T':
		securcnt++;
		{
			int bKind=0;
			long Kind;
			int eKind=0;

			assigned=sscanf(line,"T"
				" %n%1lu%n",
				&bKind,&Kind,&eKind);
			switch(assigned){
			case 0:
				MessageId=VSU_PayKindNotParsed;
				wrongpos=bKind;
				break;
			case 1:
				MessageId=S_OK;
				procedure proc(std::string("Oplata"));
				proc<<Kind<<0L<<0L;
				exec(&proc);
				break;
			}
		}
		break;
	case 'F':
		securcnt++;
		{
			int bKind=0;
			long Kind;
			int eKind=0;
			char Space;
			int eSpace=0;
			char Text[65];
			int eText=0;

			assigned=sscanf(line,"F"
				" %n%1lu%n"
				"%c%n"
				"%64c%n",
				&bKind,&Kind,&eKind,
				&Space,&eSpace,
				Text,&eText);
			switch(assigned){
			case 0:
				MessageId=VSU_FactKindNotParsed;
				wrongpos=bKind;
				break;
			case 1:
				MessageId=VSU_SpaceNotParsed;
				wrongpos=eKind;
				break;
			case 2:
				MessageId=VSU_TextNotParsed;
				wrongpos=eSpace;
				break;
			case 3:
				Text[eText-eSpace]=0;
				switch(Kind){
				case 3:
					{
						int bDanch=0;
						unsigned __int64 Danch;
						int eDanch=0;
						int sres=sscanf(Text,
							" %n%13I64u%n",
							&bDanch,&Danch,&eDanch);
						if(sres==0){
							MessageId=VSU_FactDanchNotParsed;
							wrongpos=eSpace+bDanch;
							break;
						}
						if(sres!=1)
							break;
						if(!ProcExists("BegVatFact")){
							MessageId=st_no_BegVatFact;
							wrongpos=-1;
							break;
						}
						procedure proc(std::string("BegVatFact"));
						proc<<Danch;
						exec(&proc);
					}
				default:
					MessageId=S_OK;
					ctx->factcomm[Kind]=std::string(Text);
				}
				break;
			}
		}
		break;
	case 'P':
		securcnt++;
		{
			char Space;
			int eSpace=0;
			char Text[65];
			int eText=0;

			assigned=sscanf(line,"P"
				"%c%n"
				"%64c%n",
				&Space,&eSpace,
				Text,&eText);
			switch(assigned){
			case 0:
				MessageId=VSU_SpaceNotParsed;
				break;
			case 1:
				MessageId=VSU_TextNotParsed;
				wrongpos=eSpace;
				break;
			case 2:
				MessageId=S_OK;
				Text[eText-eSpace]=0;
				procedure proc(std::string("TextComment"));
				proc<<Text;
				exec(&proc);
				break;
			}
		}
		break;
	case ';':
		throw cycleContinue();
		break;
	case '%':
		securcnt++;
		{
			int bMode=0;
			long Mode;
			int eMode=0;
			int bPerc=0;
			double Perc=0.0;
			int ePerc=0;

			assigned=sscanf(line,"%%"
				" %n%1lu%n"
				" %n%5lf%n",
				&bMode,&Mode,&eMode,
				&bPerc,&Perc,&ePerc);
			switch(assigned){
			case 0:
				MessageId=VSU_DiscKindNotParsed;
				wrongpos=bMode;
				break;
			case 1:
				MessageId=VSU_DiscPercNotParsed;
				wrongpos=bPerc;
				break;
			case 2:
				MessageId=S_OK;
				procedure proc(std::string("NacSkd"));
				proc<<Mode+6<<0<<Perc;
				exec(&proc);
				break;
			}
		}
		break;
	case 'A':
		securcnt++;
		MessageId=S_OK;
		break;
	default:
		wrongpos=0;
	}
	if(MessageId){
		out<<getmsg(this,MessageId)<<endl<<line<<endl;
		if(wrongpos>=0){
			char*ptr=(char*)_alloca(strlen(line)+1);
			int i=0;
			while(i<wrongpos){
				if(line[i]=='\t')
					ptr[i]=line[i];
				else
					ptr[i]=' ';
				i++;
			}
			ptr[i++]='^';
			ptr[i++]=0;
			out<<ptr<<endl;
		}
	}
	return MessageId;
}

//elkadrv95.doc
HRESULT CExeModule::LineFormat2(LineContext2*ctx,char*const line,unsigned&securcnt,bool&rcptclosed,bool&rcptstarted)
{
	HRESULT hr=S_OK;
	DWORD MessageId=VSU_SyntaxError;
	int wrongpos=1;
	int assigned=EOF;
	long DiscMode;
	switch(line[0]){
	case 0:
		throw cycleContinue();
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		//Продажа
		hr=CanSell(rcptstarted);
		if(SUCCEEDED(hr)){
			enum{
				wCode=4,
				wName=12,
				wPrice=8,
				wQty=8,
				wTax=1,
				wDep=2,
				wGrp=2,
			};
			int pos;
			char const*cptr=line;

			unsigned __int64 Code;
			pos=0;
			assigned=sscanf(cptr,"%4I64u%n",&Code,&pos);//wCode inside text literal
			if(assigned!=1 || pos!=wCode){
				MessageId=VSU_CodeNotParsed;
				wrongpos=cptr-line+pos;
				break;
			}
			cptr+=pos;

			char Name[wName+1];
			pos=0;
			assigned=sscanf(cptr,"%12c%n",Name,&pos);//wName inside text literal
			if(assigned!=1 || pos!=wName){
				MessageId=VSU_NameNotParsed;
				wrongpos=cptr-line+pos;
				break;
			}
			Name[wName]=0;
			cptr+=pos;

			long kop;
			pos=0;
			assigned=sscanf(cptr,"%8lu%n",&kop,&pos);//wPrice inside text literal
			if(assigned!=1 && pos!=wPrice){
				MessageId=VSU_PriceNotParsed;
				wrongpos=cptr-line+pos;
				break;
			}
			_variant_t Price;
			Price.vt=VT_CY;
			Price.cyVal.int64=kop;
			Price.cyVal.int64*=cyMul[ScaleMoney];
			cptr+=pos;
			
			long gram;
			pos=0;
			assigned=sscanf(cptr,"%8lu%n",&gram,&pos);//wQty inside string literal
			if(assigned!=1 || pos!=wQty){
				MessageId=VSU_QtyNotParsed;
				wrongpos=cptr-line+pos;
				break;
			}
			_variant_t Qty;
			Qty.vt=VT_CY;
			Qty.cyVal.int64=gram;
			Qty.cyVal.int64*=cyMul[ScaleAmount];
			cptr+=pos;
			
			long Tax=2;
			if(*cptr){
				pos=0;
				assigned=sscanf(cptr,"%1lu%n",&Tax,&pos);//wTax inside string literal
				if(assigned!=1 || pos!=wTax){
					MessageId=VSU_TaxNotParsed;
					wrongpos=cptr-line+pos;
					break;
				}
				cptr+=pos;
			}

			long Dep=1;
			if(*cptr){
				pos=0;
				assigned=sscanf(cptr,"%2lu%n",&Dep,&pos);//wDep inside string literal
				if(assigned!=1 || pos!=wDep){
					MessageId=VSU_2DepNotParsed;
					wrongpos=cptr-line+pos;
					break;
				}
				cptr+=pos;
			}

			long Grp=1;
			if(*cptr){
				pos=0;
				assigned=sscanf(cptr,"%2lu%n",&Grp,&pos);//wGrp inside string literal
				if(assigned!=1 || pos!=wGrp){
					MessageId=VSU_2GrpNotParsed;
					wrongpos=cptr-line+pos;
					break;
				}
				cptr+=pos;
			}

#if 0
			long PayKind=0;
			if(*cptr){
				pos=0;
				assigned=sscanf(cptr,"%1lu%n",&PayKind,&pos);//wPayKind inside string literal
				if(assigned!=1 || pos!=wPayKind){
					MessageId=VSU_PayKindNotParsed;
					wrongpos=cptr-line+pos;
					break;
				}
				cptr+=pos;
			}
#endif

			MessageId=S_OK;
			procedure proc(std::string("FullProd"));
			proc<<Code<<Price<<Qty<<Dep<<Grp<<Tax<<Name;
			exec(&proc);
		}
		break;
	case ':'://Комментарий
		hr=CanSell(rcptstarted);
		if(SUCCEEDED(hr)){
			MessageId=S_OK;
			if(ctx->FactCommLeft){
				ctx->factcomm[100-ctx->FactCommLeft]=std::string(line+1);
				ctx->FactCommLeft--;
			}
			else{
				procedure proc(std::string("TextComment"));
				proc<<line+1;
				exec(&proc);
			}
		}
		break;
	case '#':
		DiscMode=2;
		switch(line[1]){
		case 'A':
		case 'B':
			//Игнорируем эти команды
			MessageId=S_OK;
			break;
		case 'F':
			ctx->FactCommLeft=5;
			MessageId=S_OK;
			break;
		case 'L':
			switch(line[2]){
			case 'M':
			case 'N':
				hr=CanSell(rcptstarted);
				if(SUCCEEDED(hr)){
					unsigned __int64 Danch;
					int pos=0;
					int const wDanch=13;
					assigned=sscanf(line+3,"%13I64u%n",&Danch,&pos);//wDanch inside string literal
					if(assigned!=1 || pos!=wDanch){
						MessageId=VSU_FactDanchNotParsed;
						wrongpos=3+pos;
						break;
					}
					if(!ProcExists("BegVatFact")){
						MessageId=st_no_BegVatFact;
						wrongpos=-1;
						break;
					}
					MessageId=S_OK;
					procedure proc(std::string("BegVatFact"));
					proc<<Danch;
					exec(&proc);
				}
				break;
			default:
				wrongpos=2;
			}
			break;
		case 'O'://Задание имени оператора
			hr=rcptstarted?(wrongpos=2,VSU_OnlyBeforeReceipt):CanReport(rcptclosed);
			if(SUCCEEDED(hr)){
				_bstr_t const newCashierName=line+2;
				if(0==wcsncmp(CashierName,newCashierName,CashierName.length())){
					//Заданный кассир уже зарегистрирован
					MessageId=S_OK;
				}
				else{
					//найти кассира с таким именем в таблице
					hcdb::select sel(std::string("Oper"));
					rowset rs(sh);
					sel.result_rows=&rs;
					ccerr::exec(sess,&sel);
					while(!rs.eof()){
						_bstr_t cn=rs["Name"];
						dynarray<OLECHAR> buf(cn.length()+1);
						if(0==wcsncmp(buf,newCashierName,RTrim(cn,buf)))
							break;
						rs.move(1,rows::current);
					}
					//если кассир найден, то перерегистрироваться
					if(!rs.eof()){
						_bstr_t Machine=params.Machine.c_str();
						long Port=params.Port;
						long LogNum=params.LogNum;
						long User=params.UName;
						long Password=params.Password;
						try{
							hr=Register(Machine,Port,LogNum,_variant_t(rs["ID"]),_variant_t(rs["Pswd"]));
						}
						catch(...){
							MessageId=VSU_CannotConnectAs;
							wrongpos=2;
							hr=E_FAIL;
						}
						if(FAILED(hr)){
							hr=S_OK;
							Register(Machine,Port,LogNum,User,Password);
						}
						else
							MessageId=S_OK;
					}
					else{
						MessageId=VSU_WrongCashierName;
						wrongpos=2;
					}
				}
			}
			break;
		case 'S':
			DiscMode|=4;
		case 'P':
			switch(line[2]){
			case '+':
				DiscMode|=1;
			case '-':
				//Процентные скидки/наценки на продажу/промитог
				hr=CanSell(rcptstarted);
				if(SUCCEEDED(hr)){
					long lPerc=0;
					int pos=0;
					int const wPerc=4;
					assigned=sscanf(line+3,"%4lu%n",&lPerc,&pos);//wPerc inside string literal
					if(assigned!=1 || pos!=wPerc){
						MessageId=VSU_DiscPercNotParsed;
						wrongpos=3+pos;
						break;
					}
					_variant_t Perc;
					Perc.vt=VT_CY;
					Perc.cyVal.int64=lPerc;
					Perc.cyVal.int64*=cyMul[ScalePrc];
					MessageId=S_OK;
					procedure proc(std::string("NacSkd"));
					proc<<DiscMode<<0<<Perc;
					exec(&proc);
				}
				break;
			default:
				wrongpos=2;
			}
			break;
		case 'E'://Оплата
			hr=CanSell(rcptstarted);
			if(SUCCEEDED(hr)){
				int pos;
				char const*cptr=line+2;

				long PayKind;
				pos=0;
				int const wPayKind=1;
				assigned=sscanf(cptr,"%1lu%n",&PayKind,&pos);//wPayKind inside text literal
				if(assigned!=1 && pos!=wPayKind){
					MessageId=VSU_PayKindNotParsed;
					wrongpos=cptr-line+pos;
					break;
				}
				PayKind--;
				cptr+=pos;

				double Sum=0.0;
				if(*cptr){
					pos=0;
					assigned=sscanf(cptr,"%lf%n",&Sum,&pos);
					if(assigned!=1){
						MessageId=VSU_PaySumNotParsed;
						wrongpos=cptr-line+pos;
						break;
					}
				}

				MessageId=S_OK;
				procedure proc(std::string("Oplata"));
				proc<<PayKind<<Sum<<0L;
				exec(&proc);
			}
			break;
		default:
			wrongpos=1;
		}
		break;
	case '$':
		switch(line[1]){
		case 'F':
			switch(line[2]){
			case 'N'://Z1
				hr=rcptstarted?(wrongpos=3,VSU_OnlyBeforeReceipt):CanReport(rcptclosed);
				if(SUCCEEDED(hr)){
					MessageId=S_OK;
					procedure proc(std::string("PrintReport"));
					proc<<0L;
					exec(&proc);
				}
				break;
			case 'I'://X1
				hr=rcptstarted?(wrongpos=3,VSU_OnlyBeforeReceipt):CanReport(rcptclosed);
				if(SUCCEEDED(hr)){
					MessageId=S_OK;
					procedure proc(std::string("PrintReport"));
					proc<<10L;
					exec(&proc);
				}
				break;
			case 'O':
				break;
			case 'C'://Печать копии последнего чека
				hr=rcptstarted?(wrongpos=3,VSU_OnlyBeforeReceipt):CanReport(rcptclosed);
				if(SUCCEEDED(hr)){
					MessageId=S_OK;
					procedure proc(std::string("CopyChk"));
					exec(&proc);
				}
				break;
			default:
				wrongpos=2;
			}
			break;
		case 'H':
			//Игнорируем эти команды
			MessageId=S_OK;
			break;
		case 'V'://VoidLast
			hr=CanSell(rcptstarted);
			if(SUCCEEDED(hr)){
				MessageId=S_OK;
				procedure proc(std::string("VoidLast"));
				exec(&proc);
			}
			break;
		default:
			wrongpos=1;
		}
		break;
	default:
		wrongpos=0;
	}
	if(FAILED(hr)){
		if((hr&0xf000)==0x7000)
			out<<getmsg(this,hr,wrongpos,line)<<endl;
		MessageId=hr;
	}
	else if(MessageId){
		out<<getmsg(this,MessageId)<<endl<<line<<endl;
		if(wrongpos>=0){
			char*ptr=(char*)_alloca(strlen(line)+1);
			int i=0;
			while(i<wrongpos){
				if(line[i]=='\t')
					ptr[i]=line[i];
				else
					ptr[i]=' ';
				i++;
			}
			ptr[i++]='^';
			ptr[i++]=0;
			out<<ptr<<endl;
		}
	}
	return MessageId;
}

// auto-incrementing line number
unsigned ailinenum=0;

// line modifier and guard
class linemodguard
{
	fileitf*fs;
	boost::cmatch& mr;	// match results
	char bufres[3];
	char parambuf[128];
	int paramused;
public:
	linemodguard(fileitf*afs,boost::cmatch& amr):fs(afs),mr(amr),paramused(0){
		strcpy(bufres,"Er");
		parambuf[0]=0;
	}
	void setresult(char const txt[3]){
		strcpy(bufres,txt);
	}
	void paramf(const char* format,...){
		va_list va;
		va_start(va,format);
		paramused+=_vsnprintf(parambuf+paramused,A_SIZE(parambuf)-paramused,format,va);
		va_end(va);
	}
	~linemodguard(){
		extern unsigned long globsernum;
		unsigned long sn=globsernum;
		char bufsn[11];
		switch(mr[3].length()){
		case 6:
			sn%=1000000UL;
			sprintf(bufsn,"%06lu",sn);
			break;
		case 8:
			sn%=100000000UL;
			sprintf(bufsn,"%08lu",sn);
			break;
		case 10:
			sprintf(bufsn,"%010lu",sn);
			break;
		}
		unsigned ailn=ailinenum++;
		char buflinenum[3];
		switch(mr[4].length()){
		case 1:
			ailn%=10;
			sprintf(buflinenum,"%01u",ailn);
			break;
		case 2:
			ailn%=100;
			sprintf(buflinenum,"%02u",ailn);
			break;
		}
		char buf[32+A_SIZE(parambuf)];
		fs->updateline(buf,
			sprintf(buf,"%s,%s,%s,%s,%s;%s",
			mr[1].str().c_str(),
			mr[2].str().c_str(),
			bufsn,
			buflinenum,
			bufres,
			parambuf)
			);
	}
};

// генерирует код товара, используя имя товара и номер налоговой группы
// данная реализация использует алгоритм md5
// возвращает 47-битный сгенерированный код товара (6-ти байтовое положительное число)
static unsigned __int64 gethashcode(std::string const& name, std::string const& tax, CExeModule::extra_t const extra)
{
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx,(const unsigned char*)name.c_str(),name.length());
	MD5_Update(&ctx,(const unsigned char*)tax.c_str(),tax.length());
	MD5_Update(&ctx,&extra,sizeof(extra));
	union{
		unsigned __int64 res;
		unsigned char resbuf[16];
	};
	MD5_Final(resbuf,&ctx);
	res&=0x00007fffffffffff;
	return res;
}

static unsigned __int64 gethashcode(boost::cmatch::const_reference name, boost::cmatch::const_reference tax, CExeModule::extra_t const extra)
{
	return gethashcode(name.str(), tax.str(), extra);
}

//H,1,______,_,__;
static boost::regex pat_pfx( "^(\\w),(\\d+),(_{6}|_{8}|_{10}),(_{1,2}),(_{2});" );

//Параметры команды A (отчёт)
//;;<Код>;;;
static boost::regex pat_A( "^;;(X|Z);;;" );

//Параметры команды C (отчёт)
//<Код>;<Сума>;;;;
static boost::regex pat_C( "^(0|1);(\\d{1,2}\\.\\d{2});;;;" );

//Параметры команды I (внос/вынос)
//<Код>;<Сума>;;;;
static boost::regex pat_I( "^(0|1);(\\d{1,}\\.\\d{2});;;;" );

//Параметры команды P (печать комментария)
//<Ред 1>;<Ред 2>;<Ред 3>;<Ред 4>;<Ред 5>;
static boost::regex pat_P( "^([^;]*);([^;]*);([^;]*);([^;]*);([^;]*);" );

//Параметры команды S (продажа)
//<Име>;<Цена>;<Кол>;<Щ>;<Г>;<ДГ>;0;0;
static boost::regex pat_S( "^([^;]+);(\\d{1,6}\\.\\d{2});(\\d{1,5}\\.\\d{3});(\\d);(\\d{1,2});(\\d);0;0;" );
enum{
	pats_name=1,
	pats_price=2,
	pats_qty=3,
	pats_dept=4,
	pats_group=5,
	pats_tax=6,
};

//Параметры команды T (оплата/промитог)
//<Код>;<Сума>;;;;
//также может быть пустая строка, что означает закрытие чека наличными
static boost::regex pat_T( "^([0-3]);(\\d{1,}\\.\\d{2});;;;" );
static boost::regex pat_T4( "^4;;;;;" );

//Параметры команды N (получить номер чека и номер кассира)
//_;__;____;
static boost::regex pat_N( "_;__;____;" );

HRESULT CExeModule::LineFormat3(LineContext3*ctx,char *const line,unsigned&securcnt,fileitf*fs)
{
	DWORD MessageId=VSU_SyntaxError;
	int wrongpos=1;
	int assigned=EOF;
	boost::cmatch matches;
	_RPT1(_CRT_WARN,"%s\n",line);
	if(boost::regex_search(line, matches, pat_pfx)){
		_RPT0(_CRT_WARN," - successfully matched\n");
#ifdef _DEBUG
		for(boost::cmatch::const_iterator it=matches.begin();it!=matches.end();it++){
			_RPT2(_CRT_WARN,"\t%d\t%s\n",it->length(),it->str().c_str());
		}
#endif
		// инициализируем указатель на начало параметров
		char*const param=line+matches[0].length();
		// создаём экземпляр guard-класса, который будет модифицировать строку в исходном файле
		linemodguard lmg(fs,matches);
		// match_result для параметров
		boost::cmatch mr;
		// здесь можно было бы сравнивать первый символ из matches[1], но проще взять первый символ из line
		switch(line[0]){
			/*
			if(boost::regex_search(param,mr,pat_)){
				lmg.setresult("Sd");
				procedure proc(std::string(""));
				exec(&proc);
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			*/
		case 'A'://отчёт
			if(boost::regex_search(param,mr,pat_A)){
				lmg.setresult("Sd");
				procedure proc(std::string("PrintReport"));
				switch(*mr[1].str().c_str()){
				case 'X':
					proc<<10L;
					break;
				case 'Z':
					proc<<0L;
					break;
				}
				exec(&proc);
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'C':// процентная скидка/наценка на продажу или промитог в зависимости от того, что было перед этой строкой
			if(boost::regex_search(param,mr,pat_C)){
				lmg.setresult("Sd");
				long mode;
				switch(*mr[1].str().c_str()){
				case '0':
					mode=ctx->stl?7:3;
					break;
				case '1':
					mode=ctx->stl?6:2;
					break;
				}
				double perc=atof(mr[2].str().c_str());
				procedure proc(std::string("NacSkd"));
				proc<<mode<<0<<perc;
				exec(&proc);
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'F'://освобождение кассы от команд компьютера
			//Игнорируем эти команды
			lmg.setresult("No");
			MessageId=S_OK;
			break;
		case 'H'://блокировка клавы до конца чека (необязательная)
			//Игнорируем эти команды
			lmg.setresult("No");
			MessageId=S_OK;
			break;
		case 'I'://внос/вынос денег
			if(boost::regex_search(param,mr,pat_I)){
				lmg.setresult("Sd");
				double sum=atof(mr[2].str().c_str());
				switch(*mr[1].str().c_str()){
				case '0'://вынос
					sum=-sum;
					break;
				case '1'://внос
					break;
				}
				procedure proc(std::string("InOut"));
				proc<<0L<<sum;
				exec(&proc);
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'N'://получить номер чека и номер оператора
			if(boost::regex_search(param,mr,pat_N)){
				lmg.setresult("Sd");
				procedure proc(std::string("LastChkInfo"));
				single_row sr(sh);
				proc.result_row=&sr;
				ccerr::exec(sess,&proc);
				lmg.paramf("1;%02lu;%04lu;",
					long(_variant_t(sr["NKass"])),
					long(_variant_t(sr["ChkNum"])));
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'P'://печать текста
			if(boost::regex_search(param,mr,pat_P)){
				lmg.setresult("Sd");
				boost::cmatch::const_iterator it=mr.begin();
				it++;
				while(it!=mr.end()){
					if(it->length()>0){
						CallProc("PrintComment",it->str());
					}
					it++;
				}
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'S'://продажа
			if(boost::regex_search(param,mr,pat_S)){
				lmg.setresult("Sd");
				std::string hckey=mr[pats_name].str();
				hckey+=mr[pats_tax].str();
				hashmap_t::const_iterator it=hashmap.find(hckey);
				extra_t extra=0;
				if(it!=hashmap.end())
					extra=it->second;
				int retrycnt=0;
				while(true){
					unsigned __int64 code=gethashcode(mr[pats_name],mr[pats_tax],extra);
					if(code){
						try{
							procedure proc(std::string("FullProd"));
							proc<<code;
							proc<<atof(mr[pats_price].str().c_str());	//Цена
							proc<<atof(mr[pats_qty].str().c_str());	//Кол-во
							proc<<atol(mr[pats_dept].str().c_str());
							proc<<atol(mr[pats_group].str().c_str());
							proc<<atol(mr[pats_tax].str().c_str());
							proc<<mr[pats_name].str();
							exec(&proc);
							break;
						}
						catch(ccerr::cmderror&e){
							if(extra==255||e.ecrerr!=0xC4)	//ef_DiffTaxName changed Tax or Name
								throw;
						}
					}
					extra++;
					retrycnt++;
				}
				if(retrycnt)
					hashmap[hckey]=extra;
				ctx->stl=false;
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'T'://оплата/закрытие чека/промитог
			if(*param==0||boost::regex_search(param,mr,pat_T)){
				lmg.setresult("Sd");
				long id=*param==0?0L:atol(mr[1].str().c_str());
				double sum=*param==0?0.0:atof(mr[2].str().c_str());
				procedure proc(std::string("Oplata"));
				proc<<id<<sum<<0L;
				exec(&proc);
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			else if(boost::regex_search(param,mr,pat_T4)){
				lmg.setresult("Sd");
				procedure proc(std::string("Subtotal"));
				exec(&proc);
				ctx->stl=true;
				lmg.setresult("Ok");
				MessageId=S_OK;
			}
			break;
		case 'Z'://отмена чека
			lmg.setresult("Sd");
			CallProc("VoidChk");
			lmg.setresult("Ok");
			MessageId=S_OK;
			break;
		default:
			wrongpos=0;
		}
	}
	else{
		_RPT0(_CRT_WARN," - not matched\n");
		switch(line[0]){
		case 0:
			throw cycleContinue();
			break;
		}
		wrongpos=0;
	}
	return MessageId;
}

// Sale command parameters
// S/ItemName/QTY/Item price/VAT (tax value)/department number/
namespace f2016 {
namespace pat {

	static boost::regex Sale;
	// No need to maintain separate SaleChange pattern because it exactly matches Sale pattern
	static boost::regex ValueDiscount;
	static boost::regex PercentDiscount;
	static boost::regex RcptEnd;
	static boost::regex CustomerCard;
	static boost::regex Comment;

}
}

static std::string remcomma(std::string const& s)
{
	std::string result = s;
	std::replace(result.begin(), result.end(), ',', '.');
	return result;
}

static std::string asstr(long v)
{
	char buf[16];
	sprintf(buf, "%d", v);
	return std::string(buf);
}


HRESULT CExeModule::LineFormat2016(LineContext2016*ctx, char *const line, unsigned& securcnt)
{
	if(line[0]==0){
		return S_OK;
	}
	DWORD MessageId=VSU_SyntaxError;

	// Load taxes if necessary
	if(taxes.empty()){
		hcdb::select sel(std::string("Tax"));
		rowset rs(sh);
		sel.result_rows=&rs;
		ccerr::exec(sess,&sel);
		int i = 0;
		while(!rs.eof()){
			double t=_variant_t(rs["Prc"]);
			taxes[t] = ++i;
			rs.move(1,rows::current);
		}
	}

	// Extract command identifier (string from the start of line to the first separator)
	std::string id;
	char* sep=strchr(line,cfg2016.separ);
	if(sep){
		id.assign(line,sep);
	}
	else{
		out<<emsg(st_invalid_cmdid,"Cannot parse line, please check separators: ")<<line<<endl;
		return MessageId;
	}
	cmdmap_t::const_iterator it = cfg2016.cmd.find(id);
	_RPT1(_CRT_WARN,"Extracted command identifier '%s'\n",id.c_str());
	if (it != cfg2016.cmd.end()) {
		_RPT0(_CRT_WARN," - found in the command map. Starting handler.\n");
		unsigned long ExgState=0;
		long DiscMode=0;
		// start of parameters
		char*const param=sep+1;
		// match_result for parameters
		boost::cmatch mr;
		// Select command handler by type
		switch(it->second){
		case cmdSaleChange:
			ExgState=1;
		case cmdSale:
			if(sh->tabl.valid(sh->tabl.table("BegExchange")))
				CallProc("BegExchange",ExgState);
			else if(ExgState==1)
				// This device does not support cmdSaleChange
				break;
			if(boost::regex_match(param, mr, f2016::pat::Sale)){
				double vat = atof(remcomma(mr[4].str()).c_str());
#if defined(PASS_TAX_2016)
				taxes_t::const_iterator tit = taxes.find(vat);
				if(tit == taxes.end()){
					// wrong tax specified in the input file
					break;
				}
#endif
				std::string hckey=mr[1].str();	//name
				hckey+=mr[4].str();	// tax
				hashmap_t::const_iterator it=hashmap.find(hckey);
				extra_t extra=0;
				if(it!=hashmap.end())
					extra=it->second;
				int retrycnt=0;
				while(true){
#if defined(PASS_TAX_2016)
					unsigned __int64 code=gethashcode(mr[1].str(),asstr(tit->second),extra);	// name, tax, extra
#else
					unsigned __int64 code=gethashcode(mr[1].str(),asstr(0L),extra);	// name, tax, extra
#endif
					if(code){
						try{
							procedure proc(std::string("FullProd"));
							proc<<code;
							proc<<atof(remcomma(mr[3].str()).c_str());	// price
							double price = atof(remcomma(mr[3].str()).c_str());
							double qty = atof(remcomma(mr[2].str()).c_str());
							proc<<atof(remcomma(mr[2].str()).c_str());	// qty
							proc<<atol(mr[5].str().c_str()); 	// dept
							proc<<1L;	// group
#if defined(PASS_TAX_2016)
							proc<<tit->second;	// tax
#else
							proc<<0L; // tax is defined by department
#endif
							proc<<mr[1].str();	// name
							exec(&proc);
							break;
						}
						catch(ccerr::cmderror&e){
							if(extra==255||e.ecrerr!=0xC4)	//ef_DiffTaxName changed Tax or Name
								throw;
						}
					}
					extra++;
					retrycnt++;
				}
				if(retrycnt)
					hashmap[hckey]=extra;
				MessageId=S_OK;
			}
			break;
		case cmdADiscStl:
			DiscMode|=4;
		case cmdADiscItem:
			if(boost::regex_match(param, mr, f2016::pat::ValueDiscount)){
				//if((DiscMode&4)!=0){
				//	procedure proc(std::string("Subtotal"));
				//	exec(&proc);
				//}
				std::string s = mr[1].str();
				std::replace( s.begin(), s.end(), ',', '.');
				double sum = atof(s.c_str());
				procedure proc(std::string("NacSkd"));
				proc<<DiscMode<<sum<<0;
				exec(&proc);
				MessageId=S_OK;
			}
			break;
		case cmdPDiscStl:
			DiscMode|=4;
		case cmdPDiscItem:
			DiscMode|=2;
			if(boost::regex_match(param, mr, f2016::pat::PercentDiscount)){
				//if((DiscMode&4)!=0){
				//	procedure proc(std::string("Subtotal"));
				//	exec(&proc);
				//}
				std::string s = mr[1].str();
				std::replace( s.begin(), s.end(), ',', '.');
				double perc = atof(s.c_str());
				procedure proc(std::string("NacSkd"));
				proc<<DiscMode<<0<<perc;
				exec(&proc);
				MessageId=S_OK;
			}
			break;
		case cmdRcptEnd:
			if(boost::regex_match(param, mr, f2016::pat::RcptEnd)){
				std::string s = mr[1].str();
				std::replace( s.begin(), s.end(), ',', '.');
				double Sum = atof(s.c_str());
				long PayKind = 0L;
				s = mr[2].str();
				if(s.length() > 0){
					PayKind = atol(s.c_str()) - 1;
				}
				procedure proc(std::string("Oplata"));
				proc<<PayKind<<Sum<<0L;
				exec(&proc);
				MessageId=S_OK;
			}
			break;
		case cmdCustomerCard:
			if(boost::regex_match(param, mr, f2016::pat::CustomerCard)){
				CallProc("SetFCode", mr[1].str());
				MessageId=S_OK;
			}
			break;
		case cmdComment:
			if(boost::regex_match(param, mr, f2016::pat::Comment)){
				CallProc("TextComment", mr[1].str());
				MessageId=S_OK;
			}
			break;
		}
	}
	else {
		_RPT0(_CRT_WARN," - not matched\n");

	}
	return MessageId;
}

struct byteRow {
		std::vector<unsigned char> vec;
		int length;
};

void clearCharFromString(char * line, int lineLength)
{
	int i,j,k;
	if (NULL == line)
		return;
	if (!isExcludeCharacterDetected) {
		return;
	}

	std::vector<byteRow> excludeData;

	// Parse binary string via 0x43 delimiter byte
	// Try to find the all denied sequences of bytes
	BYTE delimiter = 0x43;
	int counter = 0;
	int sequence = 0;
	excludeData.push_back(byteRow());
	for (i = 0; i < sizeof(_Module.ExcludeCharacters); i++) {
		if (_Module.ExcludeCharacters[i] == 0x00) {
			break;
		}
		if (_Module.ExcludeCharacters[i] != delimiter) {
			counter++;
			excludeData[sequence].vec.push_back(_Module.ExcludeCharacters[i]);
		}
		else {
			excludeData[sequence].length = counter;
			excludeData.push_back(byteRow());
			sequence++;
			counter = 0;
		}
	}
	excludeData[sequence].length = counter;

	// Find all index of the string which should be removed
	std::vector<int> removeIndexes;

	// Got through all sequences and make filtration of the line's bytes
	for (i = 0; i < excludeData.size(); i++) {
		for (j = 0; j < lineLength - excludeData[i].length; j++) {

			// Check if the current subset of bytes is equal to the denied sequence
			bool isSequenceDetected = true;
			for (k = j; k < j + excludeData[i].length; k++) {
				if (line[k] != excludeData[i].vec[k - j] && line[k] != excludeData[i].vec[k - j] + 0xFFFFFF00) {
					isSequenceDetected = false;
				}
			}
			if (isSequenceDetected) {
				for (k = j; k < j + excludeData[i].length; k++) {
					removeIndexes.push_back(k);
				}
			}
		}
	}

	// Remove duplicate indexes
	std::sort(removeIndexes.begin(), removeIndexes.end());
	removeIndexes.erase(std::unique(removeIndexes.begin(), removeIndexes.end() ), removeIndexes.end());
	
	for (i = 0; i < removeIndexes.size(); i++) {
		char msgbuf[2];
		sprintf(msgbuf, "%i ", removeIndexes[i]);
		OutputDebugString(msgbuf);
	}

	OutputDebugString("\n");
	
	int newLineLength = lineLength;
	
	// Count number of array shifts
	int shiftCount = 0;
	
	// Shift array of bytes due to the denied indexes
	for (i = 0; i < removeIndexes.size(); i++) {
		std::copy(line + removeIndexes[i] + 1 - shiftCount, line + newLineLength, line + removeIndexes[i] - shiftCount);
		newLineLength--;
		shiftCount++;
	}

}

HRESULT CExeModule::ProcessFile(BSTR File,std::ofstream&olg)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
//		USES_CONVERSION;
//		ifstream ifs(W2A(File));
		{
			unsigned long ul=reinterpret_cast<unsigned long>(File);
			ul^=time(NULL);
			unsigned __int64 u64;
			QueryPerformanceCounter((LARGE_INTEGER*)&u64);
			ul^=u64;
			unsigned __int8 u8=(unsigned __int8)ul;
			u8^=((unsigned __int8*)&ul)[1];
			u8^=((unsigned __int8*)&ul)[2];
			u8^=((unsigned __int8*)&ul)[3];
			if(u8<0x10)
				Security();
		}
		std::auto_ptr<fileitf> ifs;
		wchar_t*pdbf=wcsstr(File,L".dbf");
		if(pdbf){
			*pdbf=0;
			ifs=std::auto_ptr<fileitf>(new dbf(File));
			*pdbf=L'.';
		}
		else{
			pdbf=wcsstr(File,L".DBF");
			if(pdbf){
				*pdbf=0;
				ifs=std::auto_ptr<fileitf>(new dbf(File));
				*pdbf=L'.';
			}
			else{
				if(useFormat&&Format==3)
					ifs=std::auto_ptr<fileitf>(new rwfile(File));
				else
					ifs=std::auto_ptr<fileitf>(new file(File));
			}
		}
		bool error=false;
		try{
			bool rcptclosed=false;
			bool rcptstarted=false;
			union{
				LineContext*ctx;
				LineContext0*ctx0;
				LineContext1*ctx1;
				LineContext2*ctx2;
				LineContext3*ctx3;
				LineContext2016*ctx2016;
			};
			ctx=NULL;
			if(useFormat){
				switch(Format){
				case 0:ctx=new LineContext0();break;
				case 1:ctx=new LineContext1();break;
				case 2:ctx=new LineContext2();break;
				case 3:ctx=new LineContext3();break;
				case 2016:ctx=new LineContext2016();break;
				}
			}
			else
				ctx=new LineContext0();
			std::auto_ptr<LineContext> ap_ctx(ctx);
			while(*ifs&&SUCCEEDED(hr)){
				try{
					static unsigned securcnt;//счетчик строк для проверки безопасности
					char line[1<<10];
					string_in_process++;
					ifs->getline(line,sizeof(line));
#pragma message("Non portable string conversion (char* to LPTSTR)")
					if(_Module.OEMInput)
						OemToChar(line,line);
					{
						std::string vebuf=line;
						if(vebuf.size()){
							if(_Module.OEMOutput)
								CharToOem(vebuf.c_str(),(LPTSTR)vebuf.c_str());
							olg<<vebuf<<endl;
						}
					}
					clearCharFromString(line, sizeof(line));
					if(useFormat){
						switch(Format){
						case 0:
							LineFormat0(ctx0,line,error,securcnt);
							if(error)
								hr=E_FAIL;
							break;
						case 1:
							hr=LineFormat1(ctx1,line,securcnt);
							break;
						case 2:
							hr=LineFormat2(ctx2,line,securcnt,rcptclosed,rcptstarted);
							break;
						case 3:
							hr=LineFormat3(ctx3,line,securcnt,ifs.get());
							break;
						case 2016:
							hr=LineFormat2016(ctx2016,line,securcnt);
							break;
						}
					}
					else{
						LineFormat0(ctx0,line,error,securcnt);
						if(error)
							hr=E_FAIL;
					}
					if(!(securcnt&0x7f))
						Security();
				}
				catch(cycleContinue&){
				}
			}
			if(SUCCEEDED(hr)){
				LineContext::t_facture_comments::const_iterator it=ctx->factcomm.begin();
				while(it!=ctx->factcomm.end()){
					procedure proc(std::string("TextComment"));
					proc<<it->second;
					exec(&proc);
					it++;
				}
			}
			if(Format==2 && SUCCEEDED(hr) && rcptstarted){
				CallProc("EndChk");
			}
		}
		catch(cycleBreak&){
		}
	}
	return hr;
}

HRESULT CExeModule::ClrAllPlu()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		Security();
		CallProc("ClrAllPLU");
	}
	return hr;
}

HRESULT CExeModule::Call0(BSTR ProcName)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		Security();
		CallProc(std::string(_bstr_t(ProcName)));
	}
	return hr;
}

HRESULT CExeModule::Call1(BSTR ProcName,_variant_t& p1)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		Security();
		CallProc(std::string(_bstr_t(ProcName)),p1);
	}
	return hr;
}

HRESULT CExeModule::Call5(BSTR ProcName,_variant_t& p1,_variant_t& p2,_variant_t& p3,_variant_t& p4,_variant_t& p5)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		Security();
		CallProc(std::string(_bstr_t(ProcName)),p1,p2,p3,p4,p5);
	}
	return hr;
}

HRESULT CExeModule::PrintError(BSTR File,DWORD eCode)
{
	switch(eCode){
	case st_no_session_started:
		out<<emsg(eCode,"Can not handle '%1!ls!' because connection with cash register is not initiated",File)<<endl;
		break;
	case st_no_session_established:
		out<<emsg(eCode,"Can not handle '%1!ls!' because VSU is waiting for cash register connection",File)<<endl;
		break;
	case st_temporary_unavailable:
		out<<emsg(eCode,"Can not handle '%1!ls!' because cash register has suddenly disconnected",File)<<endl;
		break;
	}
	
	return E_FAIL;
}

HRESULT CExeModule::Squeeze()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		Security();
		CallProc("Squeeze");
	}
	return hr;
}

__forceinline void CExeModule::CloseSession()
{
	extern unsigned long globsernum;
	globsernum=0;
	ModifyTrayIcon(mtiRed);
	if(sess){
		parameters::devicemap::iterator dit=params.devices->find(devID(params.Port,params.LogNum,params.Machine));
		if(dit!=params.devices->end()){
			devinfo::sessionmap*sessions=&dit->second.sessions;
			for(devinfo::sessionmap::iterator sit=sessions->begin();sit!=sessions->end();sit++)
				sit->second=NULL;
			params.devices->erase(dit);
		}
		sess=NULL;
		params.LogNum=-1;
		params.Machine="";
		params.UName=-1;
		params.Password=-1;
		hashmap.clear();
	}
}

void CExeModule::InitErrorFile()
{
}

/*void CExeModule::CallProc(string const&Name,string const&Params)
{
	string cmd=string("=")+Name+Params;
	char const*const argv[2]={"",cmd.c_str()};
	params.call_proc(2,argv);
}*/

HRESULT CExeModule::CloseCheck(HRESULT fhr)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		if(FAILED(fhr))
			CallProc("VoidChk");
		else
			CallProc("EndChk");
	}
	return hr;
}

HRESULT CExeModule::FormClose(HRESULT fhr)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		if(FAILED(fhr))
			CallProc("CancelForm");
		else
			CallProc("CloseForm");
	}
	return hr;
}

HRESULT CExeModule::CommentClose(HRESULT fhr)
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr)){
		if(FAILED(fhr))
			CallProc("CloseComment");
		else
			CallProc("CloseComment");
	}
	return hr;
}

HRESULT CExeModule::OpenShift()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr))
		CallProc("SmenBegin");
	return hr;
}

HRESULT CExeModule::OpenCheck()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr))
		CallProc("BegChk");
	return hr;
}

HRESULT CExeModule::FormOpen()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr))
		CallProc("OpenForm");
	return hr;
}

HRESULT CExeModule::CommentOpen()
{
	HRESULT hr=CheckState();
	if(SUCCEEDED(hr))
		CallProc("OpenComment");
	return hr;
}

HRESULT CExeModule::CallCAP(LPCTSTR s)
{
	pIHcUniverse2 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr))
		hr=p->ClrAllPlu();
	return hr;
}

HRESULT CExeModule::CallCall0(LPCTSTR s,LPCTSTR pn)
{
	pIHcUniverse2 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr))
		hr=p->Call0(_bstr_t(pn));
	return hr;
}

HRESULT CExeModule::CallCall1(LPCTSTR s,LPCTSTR pn)
{
	pIHcUniverse2 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr))
		hr=p->Call1(_bstr_t(pn),_variant_t(s));
	return hr;
}

HRESULT CExeModule::CallCall5(LPTSTR s,LPCTSTR pn)
{
	pIHcUniverse5 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr)){
		_variant_t p1,p2,p3,p4,p5;
		LPCTSTR seps=TEXT(" \t");
		TCHAR*token=_tcstok(s,seps);
		if(token){
			p1=token;
			if(token=_tcstok(NULL,seps)){
				p2=token;
				if(token=_tcstok(NULL,seps)){
					p3=token;
					if(token=_tcstok(NULL,seps)){
						p4=token;
						if(token=_tcstok(NULL,seps)){
							p5=token;
						}
					}
				}
			}
		}
		hr=p->Call5(_bstr_t(pn),p1,p2,p3,p4,p5);
	}
	return hr;
}

HRESULT CExeModule::CallSqueeze(LPCTSTR s)
{
	pIHcUniverse2 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr))
		hr=p->Squeeze();
	return hr;
}

HRESULT CExeModule::CallUnload(LPCTSTR s)
{
	pIHcUniverse2 p;
	HRESULT hr=GetItf(&p);
	if(p&&SUCCEEDED(hr))
		hr=p->Unload();
	return hr;
}

HRESULT CExeModule::CallConnect(LPCTSTR s)
{
	HRESULT hr=S_OK;
	int charsread=0;
	_TCHAR Machine[1<<8]="";
	long Port=0,LogNum=0,User=0,Psw=0;
	while(s[0]){
		_stscanf(s,_T(" %n"),&charsread);
		s+=charsread;
		if(s[0]!=_T('-')&&s[0]!=_T('/'))
			return ERROR_BAD_FORMAT;
		s++;
		switch(*s++){
		case 'c':
		case 'C':
			if(1!=_stscanf(s,_T("%[^ \t]%n"),Machine,&charsread))
				return ERROR_BAD_FORMAT;
			break;
		case 'u':
		case 'U':
			if(1!=_stscanf(s,_T("%lu%n"),&User,&charsread))
				return ERROR_BAD_FORMAT;
			s+=charsread;
			if(*s==_T(':')){
				s++;
				if(1!=_stscanf(s,_T("%lu%n"),&Psw,&charsread))
					Psw=0;
			}
			else
				s-=charsread;
			break;
		case 'n':
		case 'N':
			if(1!=_stscanf(s,_T("%lu%n"),&LogNum,&charsread))
				return ERROR_BAD_FORMAT;
			break;
		case 'p':
		case 'P':
			if(1!=_stscanf(s,_T("%lu%n"),&Port,&charsread))
				return ERROR_BAD_FORMAT;
			s+=charsread;
			if(*s==_T(':')){
				s++;
				if(1!=_stscanf(s,_T("%lu%n"),&LogNum,&charsread))
					LogNum=0;
			}
			else
				s-=charsread;
			break;
		}
		s+=charsread;
	}
	pIHcUniverse3 p;
	hr=GetItf(&p);
	if(p&&SUCCEEDED(hr)){
		_bstr_t bt(Machine);
		hr=p->RegisterEx(bt,Port,LogNum,User,Psw);
	}
	return hr;
}

HRESULT CExeModule::CallFile(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse2 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			size_t namelen=_tcslen(name);
			if(GetFullPathName(name,A_SIZE(name)-namelen-1,name+namelen+1,NULL)<A_SIZE(name)-namelen-1){
				_bstr_t bt(name+namelen+1);
				hr=p->ProcessFile(bt);
			}
			else
				hr=E_FAIL;
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallForm(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse2 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			size_t namelen=_tcslen(name);
			if(GetFullPathName(name,A_SIZE(name)-namelen-1,name+namelen+1,NULL)<A_SIZE(name)-namelen-1){
				_bstr_t bt(name+namelen+1);
				hr=p->PrintForm(bt);
			}
			else
				hr=E_FAIL;
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallComment(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse2 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			size_t namelen=_tcslen(name);
			if(GetFullPathName(name,A_SIZE(name)-namelen-1,name+namelen+1,NULL)<A_SIZE(name)-namelen-1){
				_bstr_t bt(name+namelen+1);
				hr=p->PrintComment(bt);
			}
			else
				hr=E_FAIL;
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallVoid(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse3 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			size_t namelen=_tcslen(name);
			if(GetFullPathName(name,A_SIZE(name)-namelen-1,name+namelen+1,NULL)<A_SIZE(name)-namelen-1){
				_bstr_t bt(name+namelen+1);
				hr=p->PrintVoid(bt);
			}
			else
				hr=E_FAIL;
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallWatch(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse3 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			size_t namelen=_tcslen(name);
			if(GetFullPathName(name,A_SIZE(name)-namelen-1,name+namelen+1,NULL)<A_SIZE(name)-namelen-1){
				_bstr_t bt(name+namelen+1);
				hr=p->SetWatchDir(bt);
			}
			else
				hr=E_FAIL;
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallWildcard(LPCTSTR s)
{
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	_TCHAR name[1<<10];
	HRESULT hr=S_OK;
	if(1==_stscanf(s,(s[0]==_T('"'))?_T("\"%[^\"]\"%n"):_T("%[^ \t]%n"),name,&charsread)){
		pIHcUniverse4 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			_bstr_t bt(name);
			hr=p->SetWildcard(bt);
		}
	}
	else
		hr=ERROR_BAD_FORMAT;
	return hr;
}

HRESULT CExeModule::CallFormat(LPCTSTR s)
{
	HRESULT hr=S_OK;
	int charsread=0;
	_stscanf(s,_T(" %n"),&charsread);
	s+=charsread;
	long nProt=0;
	switch(s[0]){
	case TEXT('1'):
		nProt=1;
		break;
	case TEXT('2'):
		nProt=2;
		break;
	default:
		hr=ERROR_BAD_FORMAT;
	}
	if(hr==S_OK){
		pIHcUniverse4 p;
		hr=GetItf(&p);
		if(p&&SUCCEEDED(hr)){
			hr=p->SetFormat(nProt);
		}
	}
	return hr;
}

void CExeModule::BeforeRun(HINSTANCE hInstance)
{
	m_hInstResource=hres?hres:eres;
	params.CodePageWin=true;
	ShutDowned=false;
	Terminated=false;

//RegisterClassEx
	WNDCLASSEX wce;
	wce.cbSize=sizeof(wce);
	wce.style=0;
	wce.lpfnWndProc=::WindowProc;
	wce.cbClsExtra=0;
	wce.cbWndExtra=0;
	wce.hInstance=hInstance;
	wce.hIcon=NULL;
	wce.hCursor=NULL;
	wce.hbrBackground=NULL;
	wce.lpszMenuName=NULL;
	wce.lpszClassName=classID;
	wce.hIconSm=NULL;
	ATOM atom=RegisterClassEx(&wce);
	_ASSERTE(atom);
//CreateWindow
	hwnd=CreateWindow(
		MAKEINTATOM(atom),  // pointer to registered class name
		strID,//NULL, // pointer to window name
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,                // horizontal position of window
		CW_USEDEFAULT,                // vertical position of window
		CW_USEDEFAULT,           // window width
		CW_USEDEFAULT,          // window height
		NULL,      // handle to parent or owner window
		NULL,          // handle to menu or child-window identifier
		hInstance,     // handle to application instance
		NULL        // pointer to window-creation data
		);
	_ASSERTE(hwnd);
}

void CExeModule::MenuShutDown()
{
	if(!ShutDowned){
		dwTimeOut=dwTimeOutForced;
		ShutDowned=true;
		EnableMenuItem(hMenu,ID_M1_SHUTDOWN,MF_GRAYED);
		Unlock();
	}
}

void CExeModule::MenuTerminate()
{
	if(!Terminated){
		MenuShutDown();
		Terminated=true;
		EnableMenuItem(hMenu,ID_M1_TERMINATE,MF_GRAYED);
		//PostThreadMessage(_Module.dwThreadID, WM_QUIT, 0, 0);
		while(GetLockCount()>0)
			Unlock();
	}
}

void CExeModule::MakeStandAlone()
{
	bEmbedding=false;
	Lock();
	InstallTrayIcon(m_hInstResource);
}

void CExeModule::InitOEMs()
{
	if(!OEMInputOverriden){
		OEMInput=true;
		try{
			OEMInput=hvsu[_T("OEMInput")];
		}
		catch(hkey::regerror&){
			//ожидаемое исключение
		}
	}
	if(!OEMOutputOverriden){
		OEMOutput=true;
		try{
			OEMOutput=hvsu[_T("OEMOutput")];
		}
		catch(hkey::regerror&){
			//ожидаемое исключение
		}
	}

	DefaultFlags=0;
	try{
		DefaultFlags=long(hvsu[_T("DefaultFlags")]);
	}
	catch(hkey::regerror&){
		//ожидаемое исключение
	}
}

#include <myreg.h>

/*HMODULE CExeModule::LoadResourceModule(long lang)
{
	HMODULE hm=NULL;
	_bstr_t rll;
	TCHAR value[16];
	wsprintf(value,TEXT("%lu"),lang);
	try{
		rll=hkey(hvsu,TEXT("lang"))[value];
		hm=LoadLibrary(rll);
	}
	catch(hkey::regerror&){}
	return hm;
}*/

HMODULE CExeModule::LoadResourceModule(long lang)
{
	HMODULE hm=NULL;
	TCHAR szFile[MAX_PATH];
	//GUID для vsu.exe
	TCHAR szComponent[]=TEXT("{1E55EC5A-5CC9-494D-8531-F0E0F97FEA89}");
	//определяем идентификатор продукта и установлен ли продукт
	TCHAR szProduct[39];
	UINT hr=MsiGetProductCode(szComponent,szProduct);
	if(hr==ERROR_SUCCESS){
		//в szProduct получен GUID продукта
		TCHAR szFeature[]=TEXT("vsuresXXXXXXXX");
		wsprintf(szFeature,TEXT("vsures%X"),lang);
		wsprintf(szComponent,TEXT("{%08X-2DDF-4E4F-9B61-86C50EBF8FFD}"),lang);
		DWORD size=A_SIZE(szFile);
		hr=MsiProvideComponent(szProduct,szFeature,szComponent,0,szFile,&size);
		if(hr==ERROR_SUCCESS){
			hm=LoadLibrary(szFile);
		}
	}
	if(hm==NULL){
		//продукт не установлен
		wsprintf(szFile,TEXT("resources\\%d\\vsures.dll"),lang);
		hm=LoadLibrary(szFile);
	}
	return hm;
}

void CExeModule::exec(procedure*proc)
{
	single_row sr(sh);
	proc->result_row=&sr;
	ccerr::exec(sess,proc);
}

void CExeModule::CallProc(std::string const&Name)
{
	procedure proc(Name);
	exec(&proc);
}

HRESULT CExeModule::CanSell(bool&rcptstarted)
{
	HRESULT hr=S_OK;
	while(!rcptstarted){
		hr=0xc0005100 | ecrp("BegChk");
		switch(hr & 0xff){
		case 0x00:
			rcptstarted=true;
			hr=S_OK;
			break;
		case 0xf9://Кассир не зарегистрирован
			hr=0xc0005100 | ecrp("SmenBegin");
			switch(hr & 0xff){
			case 0x00:
				break;
			default:
				return hr;
			}
			break;
		case 0xfd://Нефискальный чек уже открыт
			ecrp("CloseComment");
			break;
		case 0xfe://Чек уже открыт
			ecrp("VoidChk");
			break;
		default:
			return hr;
		}
	}
	return hr;
}

HRESULT CExeModule::CanReport(bool&rcptclosed)
{
	if(!rcptclosed){
		ecrp("VoidChk");
		rcptclosed=true;
	}
	return S_OK;
}

HRESULT CExeModule::ecrp(char const*Name)
{
	std::string sn(Name);
	procedure proc(sn);
	try{
		exec(&proc);
		return 0;
	}
	catch(ccerr::cmderror&e){
		return e.ecrerr;
	}
}

void CExeModule::ExecuteRegConfiguration()
{
	DWORD dwThreadId;
	CloseHandle(CreateThread(NULL,0,tsr(&CExeModule::RegConfiguration),this,0,&dwThreadId));
}

DWORD WINAPI CExeModule::RegConfiguration()
{
	struct Co Co;
	//OEMInput OEMOutput
	InitOEMs();
	//DefConn dc*
	try{
		if(bool(hvsu[TEXT("DefConn")])){
			long m_nNumber=1;
			long m_nPort=1;
			long m_nUser=1;
			long m_nPassword=0;
			switch(long(hvsu[TEXT("dcProtocol")])){
			case 0:
				m_nPort=0;
				m_nNumber=hvsu[_T("dcNumber")];
				break;
			case 1:
				m_nNumber=hvsu[_T("dcNumber")];
				m_nPort=hvsu[_T("dcPort")];
				break;
			case 2:
				m_nNumber=0;
				m_nPort=hvsu[_T("dcPort")];
				break;
			}
			try{
				m_nUser=hvsu[_T("dcUser")];
				m_nPassword=hvsu[_T("dcPassword")];
			}
			catch(hkey::regerror&){}
			pIHcUniverse3 p;
			HRESULT hr=GetItf(&p);
			if(p&&SUCCEEDED(hr)){
				_bstr_t bt;//(Machine);
				try{bt=hvsu[_T("dcLocation")];}
				catch(hkey::regerror&){}
				hr=p->RegisterEx(bt,m_nPort,m_nNumber,m_nUser,m_nPassword);
			}
		}
	}
	catch(hkey::regerror&){}
	//Format
	if(alterfunc){
		Format = 2016;
	}
	else{
		try{Format=hvsu[TEXT("Format")];}
		catch(hkey::regerror&){}
	}
	//UseWatch
	//WatchDir
	//WatchMask
	try{
		if(bool(hvsu[TEXT("UseWatch")])){
			WatchDir=hvsu[TEXT("WatchDir")];
			addSlash(_Module.WatchDir);
			WatchMask=hvsu[TEXT("WatchMask")];
		}
	}
	catch(hkey::regerror&){}
	
	// Read all exclude characters from the registry
	try {
		DWORD dwBufSize = sizeof(ExcludeCharacters);
		DWORD error = RegQueryValueEx(hvsu,"ExcludeCharacters",0,0, (LPBYTE)ExcludeCharacters, &dwBufSize);
		if (error == ERROR_SUCCESS && dwBufSize > 0) {
			isExcludeCharacterDetected = true;
		}
	}
	catch(hkey::regerror&){
		
	}

	// Format 2016
	if(alterfunc){
		try{
			ErrMode = hvsu[TEXT("ErrMode")];
		}
		catch(hkey::regerror&){}

		try{
			RepeatMode = (bool)hvsu[TEXT("RepeatMode")];
		}
		catch(hkey::regerror&){}

		try{
			std::string separator = _bstr_t(hvsu[TEXT("2016-separator")]);
			if(separator.length()>0){
				cfg2016.separ = separator.c_str()[0];
			}
		}
		catch(hkey::regerror&){}

		{
			std::string sep = "\\";
			sep += cfg2016.separ;

			// optional separator or whitespace at the end of string
			// [ \t\/]*$
			std::string oseos = "[ \\t";
			oseos += sep;
			oseos += "]*$";

			// optional whitespace before numeric values or before separators
			std::string optws = "\\s*";

			std::string rex;

			// item name
			rex = "^([^";
			rex += sep;
			rex += "]*)";
			rex += sep;
			// qty
			rex += optws;
			rex += "(\\d+(?:[\\.\\,]\\d+)?)";
			rex += optws;
			rex += sep;
			// price
			rex += optws;
			rex += "(\\d+(?:[\\.\\,]\\d+)?)";
			rex += optws;
			rex += sep;
			// VAT (tax value)
			rex += optws;
			rex += "(\\d+(?:[\\.\\,]\\d+)?)";
			rex += optws;
			rex += sep;
			// department number
			rex += optws;
			rex += "(\\d+(?:[\\.\\,]\\d+)?)";
			rex += oseos;
			f2016::pat::Sale = rex.c_str();
			
			rex = "^";
			rex += optws;
			rex += "(\\d+[\\.\\,]\\d+)";
			rex += oseos;
			f2016::pat::ValueDiscount = rex.c_str();

			rex = "^";
			rex += optws;
			rex += "(\\d+(?:[\\.\\,]\\d+)?)";
			rex += oseos;
			f2016::pat::PercentDiscount = rex.c_str();

			rex = "^";
			rex += optws;
			rex += "(\\d+[\\.\\,]\\d+)";
			rex += optws;
			rex += sep;
			rex += optws;
			rex += "(\\d?)";
			rex += oseos;
			f2016::pat::RcptEnd = rex.c_str();

			rex = "^";
			rex += optws;
			rex += "(\\d{1,18})";
			rex += oseos;
			f2016::pat::CustomerCard = rex.c_str();

			rex = "^([^";
			rex += sep;
			rex += "]*)";
			rex += oseos;
			f2016::pat::Comment = rex.c_str();
		}

		cfg2016.cmd.clear();
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-Sale")]))] = cmdSale;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["S"]=cmdSale;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-SaleChange")]))] = cmdSaleChange;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["SC"]=cmdSaleChange;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-ADiscStl")]))] = cmdADiscStl;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["TD"]=cmdADiscStl;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-ADiscItem")]))] = cmdADiscItem;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["PD"]=cmdADiscItem;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-PDiscStl")]))] = cmdPDiscStl;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["T"]=cmdPDiscStl;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-PDiscItem")]))] = cmdPDiscItem;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["P"]=cmdPDiscItem;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-RcptEnd")]))] = cmdRcptEnd;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["CR"]=cmdRcptEnd;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-CustomerCard")]))] = cmdCustomerCard;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["FC"]=cmdCustomerCard;
		}
		try{
			cfg2016.cmd[std::string(_bstr_t(hvsu[TEXT("2016-Comment")]))] = cmdComment;
		}
		catch(hkey::regerror&){
			cfg2016.cmd["C"]=cmdComment;
		}
	}

	return 0;
}

std::string CExeModule::emsg(DWORD ecode,LPCTSTR defmsg,...)
{
	va_list vl;
	va_start(vl,defmsg);
	globerr=ecode;
	struct tstr{
		LPCTSTR tr;
		tstr():tr(NULL){}
		~tstr(){if(tr)LocalFree(HLOCAL(tr));}
	}ts;
	DWORD hr=0;
	if(hres!=NULL)
		hr=xvFormatMessage(ts.tr,hres,ecode,vl);
	if(!hr && eres!=NULL)
		hr=xvFormatMessage(ts.tr,eres,ecode,vl);
	if(!hr && defmsg!=NULL)
		hr=xvFormatMessage(ts.tr,defmsg,ecode,vl);
	va_end(vl);
	return ts.tr;
}

bool CExeModule::ProcExists(char const*name)
{
	return sh->tabl.valid(sh->tabl.table(name));
}

getmsg::getmsg(CExeModule const*em,DWORD mid,...):lpMsgBuf(NULL),hr(0)
{
	va_list vl;
	va_start(vl,mid);
	if(em->hres!=NULL)
		hr=xvFormatMessage(lpMsgBuf,em->hres,mid,vl);
	if(!hr && em->eres!=NULL)
		hr=xvFormatMessage(lpMsgBuf,em->eres,mid,vl);
	va_end(vl);
}

void CExeModule::report(notifier* n)
{
	query* q = sess->current_query;
	//const shema* s = sess->get_shema();
	if(q){
		// обработка запросов
	}
	else{
		//касса включена либо выключена
		if(sess->is_on()){
			Online=true;
			ModifyTrayIcon(mtiGreen);
		}
		else{
			Online=false;
			ModifyTrayIcon(mtiYellow);
		}
	}
}

HRESULT CExeModule::CheckState()
{
	if(!sess){
		out<<emsg(st_no_session,"Connection with Cash register is not established")<<endl;
		return E_FAIL;
	}
	return S_OK;
}

Config2016::Config2016():
	separ('/')
{
	// Below is just default identifiers
	// They should be changed with values from configuration
	cmd["S"]=cmdSale;
	cmd["SC"]=cmdSaleChange;
	cmd["TD"]=cmdADiscStl;
	cmd["PD"]=cmdADiscItem;
	cmd["T"]=cmdPDiscStl;
	cmd["P"]=cmdPDiscItem;
	cmd["CR"]=cmdRcptEnd;
	cmd["FC"]=cmdCustomerCard;
	cmd["C"]=cmdComment;
}
