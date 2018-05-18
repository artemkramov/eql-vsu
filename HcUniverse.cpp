// HcUniverse.cpp : Implementation of CHcUniverse
#include "stdafx.h"
#include "Vsu.h"
#include "HcUniverse.h"
#include "vsures\vsumc.h"

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CHcUniverse

/*class vsuerr:public ofstream
{
};

vsuerr ve;*/

extern DWORD globerr;
extern unsigned string_in_process;
unsigned long globsernum=0;

ostringstream ve;

ostream &out=ve;

_RecordsetPtr globrst;

STDMETHODIMP CHcUniverse::Register(BSTR Machine, long LogNum, long User, long Password)
{
	olgguard og(this);
	olg<<"Register("<<LogNum<<','<<User<<")"<<endl;
	DeleteErrors();
	mi=miRegister;
	fMachine=Machine;
	fPort=0;
	fLogNum=LogNum;
	fUser=User;
	fPassword=Password;
	return OutErrors(Execute());
}

STDMETHODIMP CHcUniverse::RegisterEx(BSTR Machine, LONG Port, LONG LogNum, LONG User, LONG Password)
{
	DeleteErrors();
	mi=miRegister;
	fMachine=Machine;
	fPort=Port;
	fLogNum=LogNum;
	fUser=User;
	fPassword=Password;
	return OutErrors(Execute());
}

HRESULT CHcUniverse::genericReceipt(BSTR File,methodid Open,methodid Close)
{
	DeleteErrors();
	if(_Module.Format!=2){
OpenCheck:
		mi=Open;
		HRESULT hr0=Execute();
		if(FAILED(hr0)){
			if(ecrerr==0xf9){
				mi=miOpenShift;
				hr0=Execute();
				if(SUCCEEDED(hr0))
					goto OpenCheck;
			}
			else if(ecrerr==0xfd||ecrerr==0xfe){
				mi=Close;
				fhr=hr0;
				hr0=Execute();
#ifdef DIMABUG1
				if(FAILED(hr0)&&ecrerr==0xda){
					hr0=S_OK;
				}
				else
					goto OpenCheck;
#else
				if(SUCCEEDED(hr0))
					goto OpenCheck;
#endif
			}
		}
		if(FAILED(hr0)){
			UpdateErrFile(File);
			return OutErrors(hr0);
		}
	}
	ve.str("");//если там и были ошибки, то они уже исправлены
	mi=miProcessFile;
	fFile=File;
	HRESULT hr=Execute();
	if(_Module.Format!=2){
		unsigned lecrerr=ecrerr;
		mi=Close;
		fhr=hr;
		HRESULT hr2=Execute();
#ifdef DIMABUG1
		if(FAILED(hr2)&&ecrerr==0xda){
			hr2=S_OK;
		}
#endif
		UpdateErrFile(File);
		if(FAILED(hr))
			ecrerr=lecrerr;
		return OutErrors(FAILED(hr)?hr:hr2);
	}
	return OutErrors(hr);
}

STDMETHODIMP CHcUniverse::PrintForm(BSTR File)
{
	olgguard og(this);
	olg<<"PrintForm(File)"<<endl;
	return genericReceipt(File,miFormOpen,miFormClose);
}

STDMETHODIMP CHcUniverse::PrintComment(BSTR File)
{
	olgguard og(this);
	olg<<"PrintComment(File)"<<endl;
	return genericReceipt(File,miCommentOpen,miCommentClose);
}

STDMETHODIMP CHcUniverse::ProcessFile(BSTR File)
{
	struct useFormatGuard
	{
		bool saved;
		bool&b;
		useFormatGuard(bool&v):b(v){saved=b;b=true;}
		~useFormatGuard(){b=saved;}
	}fbskhfm(_Module.useFormat);
	olgguard og(this);
	olg<<"ProcessFile(File)"<<endl;
	return genericReceipt(File,miOpenCheck,miEndChk);
}

STDMETHODIMP CHcUniverse::PrintVoid(BSTR File)
{
	olgguard og(this);
	olg<<"PrintVoid(File)"<<endl;
	return genericReceipt(File,miNothing,miEndChk);
}

STDMETHODIMP CHcUniverse::ClrAllPlu()
{
	olgguard og(this);
	olg<<"ClrAllPlu()"<<endl;
	DeleteErrors();
	mi=miClrAllPlu;
	return OutErrors(Execute());
}

STDMETHODIMP CHcUniverse::Squeeze()
{
	olgguard og(this);
	olg<<"Squeeze()"<<endl;
	DeleteErrors();
	mi=miSqueeze;
	return OutErrors(Execute());
}

HRESULT CHcUniverse::Execute()
{
	HRESULT hr=E_FAIL;
	ecrerr=0;
#if defined(Debug_ErrMsg)
	_TCHAR emsg[256];
#endif
	try{
		_Module.InitErrorFile();
		switch(mi){
		case miRegister:
			hr=_Module.Register(fMachine,fPort,fLogNum,fUser,fPassword);
			break;
		case miProcessFile:
			hr=_Module.ProcessFile(fFile,olg);
			break;
		case miClrAllPlu:
			hr=_Module.ClrAllPlu();
			break;
		case miEndChk:
			hr=_Module.CloseCheck(fhr);
			break;
		case miOpenShift:
			hr=_Module.OpenShift();
			break;
		case miOpenCheck:
			hr=_Module.OpenCheck();
			break;
		case miSqueeze:
			hr=_Module.Squeeze();
			break;
		case miCall0:
			hr=_Module.Call0(fFile);
			break;
		case miCall1:
			hr=_Module.Call1(fFile,fp1);
			break;
		case miCall5:
			hr=_Module.Call5(fFile,fp1,fp2,fp3,fp4,fp5);
			break;
		case miFormOpen:
			hr=_Module.FormOpen();
			break;
		case miFormClose:
			hr=_Module.FormClose(fhr);
			break;
		case miCommentOpen:
			hr=_Module.CommentOpen();
			break;
		case miCommentClose:
			hr=_Module.CommentClose(fhr);
			break;
		case miPrintError:
			hr=_Module.PrintError(fFile,long(fp1));
			break;
		case miNothing:
			hr=S_OK;
			break;
		}
	}
	catch(basic_session::error&e){
		char*msg[]={
			"ok",
			"noinit",
			"off",
			"flush_",
			"reinit",
			"fatal",
			"on",
			"nodata",
		};
		int i=e;
		_RPT2(_CRT_WARN,"basic_session::error %i %s\n",i,msg[i]);
		out			<<"basic_session::error "<<i<<" "<<msg[i]<<endl;
	}
	catch(ccerr::cmderror&e){
		ecrerr=e.ecrerr;
		out<<e;
#if defined(Debug_ErrMsg)
		_tcscpy(emsg,LPTSTR(e));
#endif
	}
	catch(ccerr::pdwerror&e){
		out<<e;
	}
	catch(_com_error&ce){
		_RPT1(_CRT_WARN,"\n%x\n",ce.Error());
		_RPT1(_CRT_WARN,"%s\n",ce.ErrorMessage());
		out<<hex<<ce.Error()<<endl<<ce.ErrorMessage()<<endl;
	}
	catch(...){
		out<<"Unknown fatal error "<<endl;
	}
#if defined(Debug_ErrMsg)
	if(ecrerr){
		_TCHAR buf[64];
		_stprintf(buf,_T("emsg: %s\n"),emsg);
		OutputDebugString(buf);
	}
#endif
	return hr;
}

static void kill_lines(LPTSTR s)
{
	LPTSTR first(s);
	LPTSTR last(NULL);
	LPTSTR out(s);
	while(*s){
		switch(*s){
		case 0x0d:
			*s=_T(' ');
			break;
		case _T('\n'):
			if(last){
				memcpy(out,first,(last-first+1)*sizeof(*s));
				out+=last-first+1;
				*out++=_T('\n');
			}
			first=s+1;
			last=NULL;
			break;
		case _T(' '):
		case _T('\t'):
			break;
		default:
			last=s;
		}
		s++;
	}
	::memset(out,0,(s-out)*sizeof(*s));
}

static void kill_endl(LPTSTR s)
{
	while(*s){
		if(*s=='\n')
			*s=' ';
		s++;
	}
}

HRESULT CHcUniverse::OutErrors(HRESULT hr)
{
	HRESULT rhr=hr;
	if(ecrerr)
		rhr=0xC0005100|ecrerr;
	else if(FAILED(globerr))
		rhr=globerr;
	//позаботимся об уничтожении ссылок
	_RecordsetPtr rst(globrst);
	globrst=NULL;
	/*if(FAILED(hr))*/{
		string vebuf=ve.str();
		ve.str("");
		if((rhr&0xFFFFFF00)==0xC0005100 && vebuf.length()==0){
			ERR::const_iterator it=_Module.sh->err.find(rec_ERR(rhr&0xFF));
			if(_Module.sh->err.valid(it)){
				vebuf=getmsg(&_Module,VSU_ECRError,it->describe.c_str()).lpMsgBuf;
			}
		}
		//вставим серийный номер и код ошибки в сообщение об ошибке
		ve<<globsernum<<";0x"<<hex<<rhr<<dec<<";"<<endl;
		unsigned lenline1=ve.str().size();
		if(!bool(rst)&&string_in_process)
			ve<<"Line "<<string_in_process<<": ";
		ve<<vebuf;
		vebuf=ve.str();
#pragma message("Non portable string conversion (char* to LPTSTR)")
		if(vebuf.size()){
			if(_Module.OEMOutput)
				CharToOem(vebuf.c_str(),(LPTSTR)vebuf.c_str());
			kill_lines((LPTSTR)vebuf.c_str());
			vebuf=vebuf.substr(0,::strlen(vebuf.c_str()));
		}
		ve.str("");
		{
			if(_Module.Binary){
				ofstream ofs(GetErrFile(),ios_base::out|ios_base::trunc|ios_base::binary);
				unsigned long ul1=globsernum?globsernum^0xbeef9563^rhr:0;
				unsigned long ul2=rhr?rhr^0xcafe1747:0;
				ofs.write((const char*)&ul1,sizeof(ul1));
				ofs.write((const char*)&ul2,sizeof(ul2));
				unsigned short crlf=0x0a0d;
				ofs.write((const char*)&crlf,sizeof(crlf));
			}
			ofstream ofs(GetErrFile(),ios_base::out|ios_base::app);
			if(_Module.Binary){
				ofs<<vebuf.substr(lenline1);
				olg<<vebuf.substr(lenline1);
			}
			else{
				ofs<<vebuf;
				olg<<vebuf;
			}
		}
		if(rst){
			if(FAILED(hr)){
				FieldPtr f2(rst->Fields->GetItem(2L));
				//выведем ошибки
				_RPT1(_CRT_WARN,"%d\n",f2->Type);
				_RPT1(_CRT_WARN,"%d\n",f2->DefinedSize);
				if(rst->GetEOF()==VARIANT_FALSE){
					if(f2->Type!=adLongVarChar/*201*/){
						kill_endl((LPTSTR)vebuf.c_str());
						if(vebuf.size()>=f2->DefinedSize){
							vebuf=vebuf.substr(0,f2->DefinedSize);
							vebuf[vebuf.size()-1]=vebuf[vebuf.size()-2]=vebuf[vebuf.size()-3]='.';
						}
					}
					f2->Value=vebuf.c_str();
				}
				if(rst->GetBOF()==VARIANT_FALSE)
					rst->MovePrevious();
				while(rst->GetBOF()==VARIANT_FALSE){
					f2->Value=L"Cancelled";
					rst->MovePrevious();
				}
			}
		}
	}
	return rhr;
}

const char * CHcUniverse::GetErrFile()
{
	if(TmpLogFile.length()>0)
		return TmpLogFile.c_str();
	return ErrFileName.c_str();
}

universes_set universes;

CHcUniverse::CHcUniverse():ErrFileName("vsuerr.log"),LogFileName("vsu.log"),ErrFilePath("")
{
/*	HKEY hKey;
	LONG hr=RegOpenKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu"),&hKey);
	if(hr==ERROR_SUCCESS){
		_TCHAR buf[1<<10];
		LONG cbValue=sizeof(buf);
		hr=RegQueryValue(hKey,NULL,buf,&cbValue);
		if(hr==ERROR_SUCCESS){
			ErrFileName=string(...);
		}
		RegCloseKey(hKey);
	}*/
	universes.insert(this);
}

CHcUniverse::~CHcUniverse()
{
	universes.erase(this);
}

void CHcUniverse::UpdateErrFile(BSTR f)
{
}

void CHcUniverse::DeleteErrors()
{
	ecrerr=0;
	string_in_process=0;
	globerr=0;
	globrst=NULL;
	_unlink(GetErrFile());
}

STDMETHODIMP CHcUniverse::Serial(LONG * pVal)
{
	if (pVal == NULL)
		return E_POINTER;
	*pVal=globsernum;
	return S_OK;
}
STDMETHODIMP CHcUniverse::Call0(BSTR ProcName)
{
	USES_CONVERSION;
	olgguard og(this);
	olg<<"Call0("<<W2A(ProcName)<<")"<<endl;
	DeleteErrors();
	mi=miCall0;
	fFile=ProcName;
	return OutErrors(Execute());
}
STDMETHODIMP CHcUniverse::Call1(BSTR ProcName, VARIANT p1)
{
	USES_CONVERSION;
	olgguard og(this);
	olg<<"Call1("<<W2A(ProcName)<<")"<<endl;
	DeleteErrors();
	mi=miCall1;
	fFile=ProcName;
	fp1=p1;
	return OutErrors(Execute());
}
STDMETHODIMP CHcUniverse::Call5(BSTR ProcName, VARIANT p1, VARIANT p2, VARIANT p3, VARIANT p4, VARIANT p5)
{
	USES_CONVERSION;
	olgguard og(this);
	olg<<"Call5("<<W2A(ProcName)<<")"<<endl;
	DeleteErrors();
	mi=miCall5;
	fFile=ProcName;
	fp1=p1;
	fp2=p2;
	fp3=p3;
	fp4=p4;
	fp5=p5;
	return OutErrors(Execute());
}
STDMETHODIMP CHcUniverse::PrintError(BSTR File,long eCode)
{
	USES_CONVERSION;
	olgguard og(this);
	olg<<"PrintError("<<W2A(File)<<","<<eCode<<")"<<endl;
	DeleteErrors();
	mi=miPrintError;
	fFile=File;
	fp1=eCode;
	return OutErrors(Execute());
}
STDMETHODIMP CHcUniverse::SetICP(LONG Input)
{
	_Module.OEMInput=Input!=0;
	return S_OK;
}
STDMETHODIMP CHcUniverse::SetOCP(LONG Output)
{
	_Module.OEMOutput=Output!=0;
	return S_OK;
}

STDMETHODIMP CHcUniverse::Unload()
{
	olgguard og(this);
	olg<<"Unload()"<<endl;
	if(!_Module.bEmbedding)
		_Module.MenuShutDown();
	return S_OK;
}

void addSlash(_bstr_t&s)
{
	if(((wchar_t*)s)[s.length()-1]!=L'\\'){
		s+=L"\\";
	}
}

STDMETHODIMP CHcUniverse::SetWatchDir(BSTR Dir)
{
	olgguard og(this);
	olg<<"SetWatchDir(Dir)"<<endl;
	_Module.WatchDir=Dir;
	addSlash(_Module.WatchDir);
	return S_OK;
}

STDMETHODIMP CHcUniverse::SetWildcard(BSTR Dir)
{
	olgguard og(this);
	olg<<"SetWildcard(Dir)"<<endl;
	_Module.WatchMask=Dir;
	return S_OK;
}

STDMETHODIMP CHcUniverse::SetFormat(LONG nProt)
{
	olgguard og(this);
	olg<<"SetFormat("<<nProt<<")"<<endl;
	_Module.Format=nProt;
	return S_OK;
}

