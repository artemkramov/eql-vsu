// HcUniverse.h : Declaration of the CHcUniverse

#ifndef __HCUNIVERSE_H_
#define __HCUNIVERSE_H_

#include <ios>
#include <string>
#include <fstream>
#include <set>

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHcUniverse
class ATL_NO_VTABLE CHcUniverse : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHcUniverse, &CLSID_HcUniverse>,
	//public IDispatchImpl<IHcUniverse, &IID_IHcUniverse, &LIBID_VSULib>,
	//public IDispatchImpl<IHcUniverse2, &IID_IHcUniverse2, &LIBID_VSULib>,
	//public IDispatchImpl<IHcUniverse3, &IID_IHcUniverse3, &LIBID_VSULib>
	//public IDispatchImpl<IHcUniverse4, &IID_IHcUniverse4, &LIBID_VSULib>
	//public IDispatchImpl<IHcUniverse5, &IID_IHcUniverse5, &LIBID_VSULib>
	public IDispatchImpl<IHcUniverse6, &IID_IHcUniverse6, &LIBID_VSULib>
{
	void DeleteErrors();

	std::string LogFileName;
	std::ofstream olg;
	struct olgguard
	{
		CHcUniverse*pcu;
		olgguard(CHcUniverse*apcu):pcu(apcu)
		{
			/*
			if (exists && size>trashhold)
			{
				unlink vsu.bak.log;
				rename vsu.log to vsu.bak.log;
			}
			*/
			WIN32_FILE_ATTRIBUTE_DATA data;
			if(GetFileAttributesEx(pcu->LogFileName.c_str(),GetFileExInfoStandard,&data)){
				if(data.nFileSizeLow>4096){
					std::string bakfn=pcu->LogFileName+".bak";
					DeleteFile(bakfn.c_str());
					MoveFile(pcu->LogFileName.c_str(),bakfn.c_str());
				}
			}
			pcu->olg.open(pcu->LogFileName.c_str(),std::ios_base::out|std::ios_base::app);
		}
		~olgguard()
		{
			pcu->olg.close();
		}
	};
	friend struct olgguard;

	std::string ErrFileName;
	std::string ErrFilePath;
	enum methodid{
		miClrAllPlu,
		miProcessFile,
		miRegister,
		miEndChk,
		miOpenShift,
		miOpenCheck,
		miSqueeze,
		miCall0,
		miCall1,
		miCall5,
		miFormOpen,
		miFormClose,
		miCommentOpen,
		miCommentClose,
		miPrintError,
		miNothing,
	};
	methodid mi;
	BSTR fFile;
	BSTR fMachine;
	long fPort;
	long fLogNum;
	long fUser;
	long fPassword;
	HRESULT fhr;
	_variant_t fp1;
	_variant_t fp2;
	_variant_t fp3;
	_variant_t fp4;
	_variant_t fp5;
	unsigned ecrerr;
	HRESULT Execute();
	void UpdateErrFile(BSTR f);
	HRESULT OutErrors(HRESULT hr);
	const char * GetErrFile();
	HRESULT genericReceipt(BSTR File,methodid Open,methodid Close);
public:
	std::string TmpLogFile;
	CHcUniverse();
	virtual ~CHcUniverse();

DECLARE_REGISTRY_RESOURCEID(IDR_HCUNIVERSE)
DECLARE_NOT_AGGREGATABLE(CHcUniverse)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHcUniverse)
 	COM_INTERFACE_ENTRY(IHcUniverse)
	COM_INTERFACE_ENTRY(IHcUniverse2)
	COM_INTERFACE_ENTRY(IHcUniverse3)
	COM_INTERFACE_ENTRY(IHcUniverse4)
	COM_INTERFACE_ENTRY(IHcUniverse5)
	COM_INTERFACE_ENTRY(IHcUniverse6)
//DEL 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch, IHcUniverse6)
END_COM_MAP()

// IHcUniverse
public:
	STDMETHOD(PrintVoid)(BSTR File);
	STDMETHOD(ClrAllPlu)();
	STDMETHOD(ProcessFile)(/*[in]*/BSTR File);
	STDMETHOD(Register)(/*[in]*/BSTR Machine,/*[in]*/long LogNum,/*[in]*/long User,/*[in]*/long Password);
// IHcUniverse2
	STDMETHOD(Squeeze)();
	STDMETHOD(Serial)(LONG * pVal);
	STDMETHOD(Call0)(BSTR ProcName);
	STDMETHOD(Call1)(BSTR ProcName, VARIANT p1);
	STDMETHOD(SetICP)(LONG Input);
	STDMETHOD(SetOCP)(LONG Output);
	STDMETHOD(PrintForm)(BSTR File);
	STDMETHOD(PrintComment)(BSTR File);
	STDMETHOD(Unload)();
// IHcUniverse3
	STDMETHOD(RegisterEx)(BSTR Machine, LONG Port, LONG LogNum, LONG User, LONG Password);
	STDMETHOD(SetWatchDir)(BSTR Dir);
// IHcUniverse4
	STDMETHOD(SetFormat)(LONG nProt);
	STDMETHOD(SetWildcard)(BSTR Mask);
// IHcUniverse5
	STDMETHOD(Call5)(BSTR Procname, VARIANT p1, VARIANT p2, VARIANT p3, VARIANT p4, VARIANT p5);
// IHcUniverse6
	STDMETHOD(PrintError)(BSTR File,long eCode);
};

typedef std::set<CHcUniverse*> universes_set;
#endif //__HCUNIVERSE_H_
