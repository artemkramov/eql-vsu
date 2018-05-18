// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__291BDFB4_E9FC_11D3_9BE8_008048C5FDF8__INCLUDED_)
#define AFX_STDAFX_H__291BDFB4_E9FC_11D3_9BE8_008048C5FDF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#define WINVER 0x500
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#pragma warning(disable:4146)
#import <msado25.tlb>
#pragma warning(default:4146)
using namespace ADODB;

#include <atlbase.h>
#include <set>
#include <string>
#include <algorithm>
#include <comdef.h>
#include <cassert>
#include <vector>
#include <deque>
#include <stack>
#include <fstream>
#include <sstream>
//#include <iostream>
#include <map>

#include <myreg.h>

#import "L2Srv.tlb"
using namespace L2SRVLib;

#include "ccerr.h"
#include <classes.h>
#include <handle.h>


class fileitf;

// Command types for year 2016 format

enum cmd2016{
	cmdSale,
	cmdSaleChange,
	cmdADiscStl,
	cmdADiscItem,
	cmdPDiscStl,
	cmdPDiscItem,
	cmdRcptEnd,
	cmdCustomerCard,
	cmdComment,
};

typedef std::map<std::string, cmd2016> cmdmap_t;

// Configuration data for year 2016 format
// Includes command identifiers + separator
class Config2016
{
public:
	char separ;
	cmdmap_t cmd;

	Config2016();
};

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule, public notifiable
{
	static char cnst_empty[2];
	static char cnst_left[2];
	static char cnst_right[2];
	__forceinline bool FreeSchema402();
	__forceinline bool FreeSchema418();
	__forceinline bool FreeKenya();
	__forceinline bool FreeHungary();
	__forceinline bool FreeMalta();
	__forceinline bool FreeGreece();
	__forceinline bool FreeRussia();
	__forceinline bool Security();
	class cycleContinue{};
	class cycleBreak{};
	struct LineContext//базовый класс контекста для обработчиков строк всех протоколов
	{
		typedef std::map<long,std::string> t_facture_comments;
		t_facture_comments factcomm;
		virtual ~LineContext(){}
	};
	struct LineContext0:public LineContext
	{
		virtual ~LineContext0(){}
	};
	struct LineContext1:public LineContext
	{
		virtual ~LineContext1(){}
	};
	struct LineContext2:public LineContext
	{
		int FactCommLeft;
		LineContext2():FactCommLeft(0){}
		virtual ~LineContext2(){}
	};
	void LineFormat0(LineContext0*ctx,char *const line,bool&error,unsigned&securcnt);
	HRESULT LineFormat1(LineContext1*ctx,char *const line,unsigned&securcnt);
	HRESULT LineFormat2(LineContext2*ctx,char *const line,unsigned&securcnt,bool&rcptclosed,bool&rcptstarted);
	HRESULT CanSell(bool&rcptstarted);
	HRESULT CanReport(bool&rcptclosed);
	HRESULT ecrp(char const*Name);
	DWORD WINAPI VSUCfg();
	DWORD WINAPI RegConfiguration();
	std::string emsg(DWORD ecode,LPCTSTR defmsg=NULL,...);
	bool ProcExists(char const*name);
	enum mtiState{
		mtiRed,
		mtiYellow,
		mtiGreen,
	};
	void ModifyTrayIcon(mtiState state);
	//void TrayIconBalloon(LPCTSTR szInfo, LPCTSTR szInfoTitle = NULL, DWORD dwFlags = NIIF_WARNING);
public:
	struct LineContext3:public LineContext
	{
		bool stl;
		LineContext3():stl(false){}
		virtual ~LineContext3(){}
	};
	HRESULT LineFormat3(LineContext3*ctx,char *const line,unsigned&securcnt,fileitf*fs);
public:
	// New format required by customers in 2016
	struct LineContext2016: public LineContext
	{
		virtual ~LineContext2016(){}
	};
	HRESULT LineFormat2016(LineContext2016*ctx, char *const line, unsigned& securcnt);
	// Command identifiers + separator
	Config2016 cfg2016;
	typedef std::map<double, long> taxes_t;
	taxes_t taxes;
public:
	// alterfunc is defined by ALTERFUNC macro 
	// if ALTERFUNC is not defined then alterfunc is zero
	// if ALTERFUNC is 1 then VSU behaves like is was required by customers in 2016
	const int alterfunc;
	long Format;
	bool useFormat;
	long ErrMode;
	bool RepeatMode;
	keo::ModuleHandle eres;
	keo::ModuleHandle hres;
	_bstr_t WatchDir;
	_bstr_t WatchMask;
	CExeModule();
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	HANDLE hMonitorThread;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;
	///////////////////////////l2srv
	HMENU hMenu;
	bool bEmbedding;
	UINT msgID;
	void BeforeRun(HINSTANCE hInstance);
	void ExecuteRegConfiguration();
	NOTIFYICONDATA nid;
	HWND hwnd;
	bool ShutDowned;
	void MenuShutDown();
	bool Terminated;
	void MenuTerminate();
	LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	void MakeStandAlone();
	///////////////////////////
	parameters params;
	session*sess;
	shema const*sh;
	long ScaleAmount;
	long ScaleMoney;
	long ScalePrc;
	_bstr_t CashierName;
	typedef unsigned char extra_t;
	typedef std::map<std::string,extra_t> hashmap_t;
	hashmap_t hashmap;
	void exec(procedure*proc);
	void main(HINSTANCE hInstance,LPTSTR lpCmdLine);
	void InitErrorFile();
	__forceinline void CloseSession();
	void CallProc(std::string const&Name);
	//1 parameters
	template<typename T1>
	void CallProc(std::string const&Name,T1 const&v1)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	//2 parameters
	template<typename T1,typename T2>
	void CallProc(std::string const&Name,T1 const&v1,T2 const&v2)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		std::string p2=pp(v2);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),p2.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	//3 parameters
	template<typename T1,typename T2,typename T3>
	void CallProc(std::string const&Name,T1 const&v1,T2 const&v2,T3 const&v3)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		std::string p2=pp(v2);
		std::string p3=pp(v3);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),p2.c_str(),p3.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	//5 parameters
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	void CallProc(std::string const&Name,T1 const&v1,T2 const&v2,T3 const&v3,T4 const&v4,T5 const&v5)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		std::string p2=pp(v2);
		std::string p3=pp(v3);
		std::string p4=pp(v4);
		std::string p5=pp(v5);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),p2.c_str(),p3.c_str(),p4.c_str(),p5.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	//7 parameters
	template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	void CallProc(std::string const&Name,T1 const&v1,T2 const&v2,T3 const&v3,T4 const&v4,T5 const&v5,T6 const&v6,T7 const&v7)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		std::string p2=pp(v2);
		std::string p3=pp(v3);
		std::string p4=pp(v4);
		std::string p5=pp(v5);
		std::string p6=pp(v6);
		std::string p7=pp(v7);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),p2.c_str(),p3.c_str(),p4.c_str(),p5.c_str(),p6.c_str(),p7.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	//9 parameters
	template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	void CallProc(std::string const&Name,T1 const&v1,T2 const&v2,T3 const&v3,T4 const&v4,T5 const&v5,T6 const&v6,T7 const&v7,T8 const&v8,T9 const&v9)
	{
		std::string pn=std::string("=")+Name;
		std::string p1=pp(v1);
		std::string p2=pp(v2);
		std::string p3=pp(v3);
		std::string p4=pp(v4);
		std::string p5=pp(v5);
		std::string p6=pp(v6);
		std::string p7=pp(v7);
		std::string p8=pp(v8);
		std::string p9=pp(v9);
		char const*const argv[]={cnst_empty,pn.c_str(),cnst_left,p1.c_str(),p2.c_str(),p3.c_str(),p4.c_str(),p5.c_str(),p6.c_str(),p7.c_str(),p8.c_str(),p9.c_str(),cnst_right};
		params.call_proc(sizeof(argv)/sizeof(argv[0]),argv,true);
	}
	virtual void report(notifier* n);
	HRESULT CheckState();
	HRESULT ClrAllPlu();
	HRESULT Call0(BSTR ProcName);
	HRESULT Call1(BSTR ProcName,_variant_t& p1);
	HRESULT Call5(BSTR ProcName,_variant_t& p1,_variant_t& p2,_variant_t& p3,_variant_t& p4,_variant_t& p5);
	HRESULT PrintError(BSTR File,DWORD eCode);
	HRESULT Squeeze();
	HRESULT ProcessFile(/*[in]*/BSTR File,std::ofstream&olg);
	HRESULT Register(/*[in]*/BSTR Machine,/*[in]*/long Port,/*[in]*/long LogNum,/*[in]*/long User,/*[in]*/long Password);
	HRESULT CloseCheck(HRESULT hr);
	HRESULT FormClose(HRESULT hr);
	HRESULT CommentClose(HRESULT hr);
	HRESULT OpenShift();
	HRESULT OpenCheck();
	HRESULT FormOpen();
	HRESULT CommentOpen();
	HRESULT CallConnect(LPCTSTR s);
	HRESULT CallFile(LPCTSTR s);
	HRESULT CallForm(LPCTSTR s);
	HRESULT CallComment(LPCTSTR s);
	HRESULT CallVoid(LPCTSTR s);
	HRESULT CallCAP(LPCTSTR s);
	HRESULT CallCall0(LPCTSTR s,LPCTSTR pn);
	HRESULT CallCall1(LPCTSTR s,LPCTSTR pn);
	HRESULT CallCall5(LPTSTR s,LPCTSTR pn);
	HRESULT CallSqueeze(LPCTSTR s);
	HRESULT CallUnload(LPCTSTR s);
	HRESULT CallWatch(LPCTSTR s);
	HRESULT CallFormat(LPCTSTR s);
	HRESULT CallWildcard(LPCTSTR s);
	bool OEMInputOverriden;
	bool OEMInput;
	bool OEMOutputOverriden;
	bool OEMOutput;
	bool Binary;
	void InitOEMs();
	bool Author;
	unsigned long DefaultFlags;
	HMODULE LoadResourceModule(long lang);
private:
	static std::string pp(std::string const& s){return s;}
	static std::string pp(unsigned long ul){
		char b[20];
		sprintf(b,"%lu",ul);
		return b;
	}
	static std::string pp(double d){
		char b[30];
		sprintf(b,"%lf",d);
		return b;
	}
	static std::string pp(unsigned __int64 ui){
		char b[40];
		sprintf(b,"%I64u",ui);
		return b;
	}
	static std::string pp(_variant_t const&v){
		return _bstr_t(v);
	}
/*	template<typename T1>
	static std::string ppp(T1 const&v1)
	{
		return std::string("( ")+pp(v1)+std::string(" )");
	}
	template<typename T1,typename T2>
	static std::string ppp(T1 const&v1,T2 const&v2)
	{
		return std::string("( ")+pp(v1)+std::string(" ")+pp(v2)+std::string(" )");
	}
	template<typename T1,typename T2,typename T3>
	static std::string ppp(T1 const&v1,T2 const&v2,T3 const&v3)
	{
		return std::string("( ")+pp(v1)+std::string(" ")+pp(v2)+std::string(" ")+pp(v3)+std::string(" )");
	}
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	static std::string ppp(T1 const&v1,T2 const&v2,T3 const&v3,T4 const&v4,T5 const&v5)
	{
		return std::string("( ")+pp(v1)+std::string(" ")+pp(v2)+std::string(" ")+pp(v3)+std::string(" ")+pp(v4)+std::string(" ")+pp(v5)+std::string(" )");
	}*/
};

struct getmsg
{
	LPTSTR lpMsgBuf;
	DWORD hr;
	getmsg(HINSTANCE hres,DWORD mid,...):lpMsgBuf(NULL),hr(0){
		va_list vl;
		va_start(vl,mid);
		hr=FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
			hres,
			mid,
			0,
			(LPTSTR)&lpMsgBuf,
			0,
			&vl);
		va_end(vl);
	}
	getmsg(CExeModule const*em,DWORD mid,...);
	~getmsg(){
		if(lpMsgBuf)
			LocalFree(lpMsgBuf);
	}
};

extern hkey hvsu;
extern CExeModule _Module;
void addSlash(_bstr_t&s);

#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__291BDFB4_E9FC_11D3_9BE8_008048C5FDF8__INCLUDED)
