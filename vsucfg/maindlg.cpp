#include "stdafx.h"
#include "resource.h"
#include "maindlg.h"
#include "atlmisc.h"
#include "chkln.h"

#define _WIN32_MSI   110
#include <msi.h>

CPPEncoding::CPPEncoding():
	vsu(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu")),
	m_nErrMode(1),
	m_nFormat(0),
	m_nInput(1),
	m_nOutput(1),
	m_nUseWatch(1),
	m_nRepeatMode(1)
{
	lstrcpy(m_szWatchDir,TEXT("C:\\DPS_SMART_S_FISCAL"));
	lstrcpy(m_szWatchMask,TEXT("R*.txt"));

	try{m_nErrMode=long(vsu[_T("ErrMode")]);}
	catch(hkey::regerror&){}
	
	try{m_nFormat=long(vsu[_T("Format")]);}
	catch(hkey::regerror&){}

	try{m_nInput=bool(vsu[_T("OEMInput")])?0:1;}
	catch(hkey::regerror&){}

	try{m_nOutput=bool(vsu[_T("OEMOutput")])?0:1;}
	catch(hkey::regerror&){}

	try{m_nUseWatch=long(vsu[_T("UseWatch")]);}
	catch(hkey::regerror&){}

	try{m_nRepeatMode=long(vsu[_T("RepeatMode")]);}
	catch(hkey::regerror&){}

	try{lstrcpy(m_szWatchDir,_bstr_t(vsu[_T("WatchDir")]));}
	catch(hkey::regerror&){}

	try{lstrcpy(m_szWatchMask,_bstr_t(vsu[_T("WatchMask")]));}
	catch(hkey::regerror&){}

	// create WatchDir if it does not exist
	CreateDirectory(m_szWatchDir,NULL);
}


static void getComponentPath(LPCTSTR szUpgradeCode, LPCTSTR szComponent, CString& result)
{
	TCHAR szProduct[39];
	if(ERROR_SUCCESS==MsiEnumRelatedProducts(szUpgradeCode,0,0,szProduct)){
		TCHAR szFile[MAX_PATH];
		DWORD size=sizeof(szFile)/sizeof(szFile[0]);
		if(MsiGetComponentPath(szProduct,szComponent,szFile,&size)>0 && size>0){
			result=szFile;
		}
	}
}


bool is_autorun(LPCTSTR image)
{
	CRegKey reg;
	LONG res = reg.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),KEY_READ);
	if(res!=ERROR_SUCCESS)
		return false;
	const DWORD bufsize=MAX_PATH+1;
	TCHAR path[bufsize];
	DWORD cnt=bufsize;
	res = reg.QueryValue(path,TEXT("vsuexe"),&cnt);
	if(res!=ERROR_SUCCESS)
		return false;
	CString fixed(image);
	if(path[0]==TEXT('"')&&image[0]!=TEXT('"')){
		fixed.Format(TEXT("\"%s\""),image);
	}
	return _tcscmp(fixed,path)==0;
}

void set_autorun(bool on,LPCTSTR image)
{
	CRegKey reg;
	LONG res = reg.Open(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),KEY_WRITE);
	if(res!=ERROR_SUCCESS)
		return;
	if(on)
		reg.SetValue(image,TEXT("vsuexe"));
	else
		reg.DeleteValue(TEXT("vsuexe"));
}


CPPDefConn::CPPDefConn():
	vsu(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu")),
	setDefConn(false),
	m_nAutoRun(0),
	m_nDefConn(0),
	m_nProt(0),
	m_nNumber(1),
	m_nPort(1),
	m_nUser(1),
	m_nPassword(0)
{
	// get vsu.exe path
	//Upgrade code для VSU
	TCHAR szUpgradeCode[]=TEXT("{4F0DBD46-0291-4E1A-A04C-69D7FE28C736}");
	//guid for vsu.exe
	TCHAR szComponent[]=TEXT("{1E55EC5A-5CC9-494D-8531-F0E0F97FEA89}");
	getComponentPath(szUpgradeCode,szComponent,vsuexepath);
	// если vsuexepath - пустая строка, значит не удалось получить путь (возможно продукт не инсталлирован)
	//TODO: в таком случае нужно запретить чекбокс, отвечающий за автозапуск
	
	lstrcpy(m_szLocation,TEXT(""));
	try{
		try{lstrcpy(m_szLocation,_bstr_t(vsu[_T("dcLocation")]));}
		catch(hkey::regerror&){}

		m_nProt=long(vsu[_T("dcProtocol")]);
		switch(m_nProt){
		case 0:
			m_nNumber=vsu[_T("dcNumber")];
			break;
		case 1:
			m_nNumber=vsu[_T("dcNumber")];
			m_nPort=vsu[_T("dcPort")];
			break;
		case 2:
			m_nNumber=0;
			m_nPort=vsu[_T("dcPort")];
			break;
		}

		try{
			m_nUser=vsu[_T("dcUser")];
			m_nPassword=vsu[_T("dcPassword")];
		}
		catch(hkey::regerror&){}

		bool DefConn=false;
		try{DefConn=vsu[_T("DefConn")];}
		catch(hkey::regerror&){
			setDefConn=true;
		}
		m_nDefConn=DefConn?1:0;
	}
	catch(hkey::regerror&){}
}

CPPDeptCodes::CPPDeptCodes():vsu_dept(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu\\dept"))
{
}

CPPFormat2016::CPPFormat2016():
	vsu(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu"))
{
}

CPPEncoding::~CPPEncoding()
{
}

CPPDefConn::~CPPDefConn()
{
}

CPPDeptCodes::~CPPDeptCodes()
{
}

CPPFormat2016::~CPPFormat2016()
{
}

BOOL CPPEncoding::OnInitDialog (HWND hwndFocus,LPARAM lParam)
{
	DoDataExchange(false);
	if(alterfunc==1){
		// Rename items to support error handling mode selection
		SetDlgItemText(IDC_E_CLASSIC, TEXT("Classic VSU behavior"));
		SetDlgItemText(IDC_E_MODE2016, TEXT("New error handling mode"));
		//::ShowWindow(::GetWindow(GetDlgItem(IDC_F_VSU), GW_HWNDPREV), SW_HIDE);
		::SetWindowText(::GetWindow(GetDlgItem(IDC_F_VSU), GW_HWNDPREV), TEXT("Error handling mode"));
		// Hide unnecessary items
		::ShowWindow(GetDlgItem(IDC_F_ELKA), SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_F_DATECS), SW_HIDE);
		
		// Hide "Repeat button" checkbox if old error mode is selected 
		if (m_nErrMode == 0) {
			::ShowWindow(GetDlgItem(IDC_CHB_REPEAT_MODE), SW_HIDE);
		}
	}
	return TRUE;
}

BOOL CPPDefConn::OnInitDialog(HWND hwndFocus,LPARAM lParam)
{
	RECT r;
	POINT*p;
	
	p=staticpos;
	::GetWindowRect(GetDlgItem(IDC_STATIC_LOGNUM),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);
	p++;
	::GetWindowRect(GetDlgItem(IDC_STATIC_LOCATION),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);
	p++;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PORT),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);

	p=editpos;
	::GetWindowRect(GetDlgItem(IDC_NUMBER),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);
	p++;
	::GetWindowRect(GetDlgItem(IDC_LOCATION),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);
	p++;
	::GetWindowRect(GetDlgItem(IDC_PORT),&r);
	p->x=r.left;
	p->y=r.top;
	ScreenToClient(p);

	m_nAutoRun=is_autorun(vsuexepath) ? 1 : 0;

	DoDataExchange(false);
	int tmp;
	OnRadio(0,IDC_RADIO_LOGNUM+m_nProt,0,tmp);
	if(alterfunc==1 && setDefConn){
		CheckDlgButton(IDC_CHB_DEFCONN, BST_CHECKED);
	}
	return TRUE;
}

BOOL CPPDeptCodes::OnInitDialog(HWND hwndFocus,LPARAM lParam)
{
	list=GetDlgItem(IDC_LIST_DEPT);
	list.SetExtendedListViewStyle(
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString tmp;
	tmp.LoadString(IDS_COLHEAD_DEPT);
	list.AddColumn(tmp,0);
	tmp.LoadString(IDS_COLHEAD_CODE);
	list.AddColumn(tmp,1);
	RECT r;
	list.GetClientRect(&r);
	list.SetColumnWidth(1,r.right-r.left-list.GetColumnWidth(0));
	for(long Dept=0;Dept<10;Dept++){
		TCHAR txtDept[16];
		wsprintf(txtDept,TEXT("%lu"),Dept);
		_variant_t varCode;
		varCode.vt=VT_CY;
		varCode.cyVal.int64=140737488355327-Dept;
		varCode.cyVal.int64*=10000;
		try{
			_variant_t tmpvarCode=vsu_dept[txtDept];
			if(tmpvarCode.vt==VT_I4)
				tmpvarCode.vt=VT_UI4;
			tmpvarCode.ChangeType(VT_CY);
			varCode=tmpvarCode;
		}
		catch(hkey::regerror&){}
		catch(_com_error&){}
		TCHAR txtCode[32];
		_stprintf(txtCode,TEXT("%I64u"),varCode.cyVal.int64/10000I64);
		list.InsertItem(Dept,txtDept);
		list.SetItemText(Dept,1,txtCode);
	}
	return TRUE;
}

struct EntityData{
	int id;
	LPCTSTR regname;
	LPCTSTR defval;
};

EntityData entityData[] = {
	{ IDS_ENTITY_SEPARATOR,
		TEXT("2016-separator"),
		TEXT("/")},
	{ IDS_ENTITY_SALE,
		TEXT("2016-Sale"), 
		TEXT("S") },
	{ IDS_ENTITY_SALECHANGE,
		TEXT("2016-SaleChange"),
		TEXT("SC") },
	{ IDS_ENTITY_ADISC_STL,
		TEXT("2016-ADiscStl"),
		TEXT("TD") },
	{ IDS_ENTITY_ADISC_ITEM,
		TEXT("2016-ADiscItem"),
		TEXT("PD") },
	{ IDS_ENTITY_PDISC_STL,
		TEXT("2016-PDiscStl"),
		TEXT("T") },
	{ IDS_ENTITY_PDISC_ITEM,
		TEXT("2016-PDiscItem"),
		TEXT("P") },
	{ IDS_ENTITY_ENDRCPT,
		TEXT("2016-RcptEnd"),
		TEXT("CR") },
	{ IDS_ENTITY_CCARD,
		TEXT("2016-CustomerCard"),
		TEXT("FC") },
	{ IDS_ENTITY_COMMENT,
		TEXT("2016-Comment"),
		TEXT("C") },
};

BOOL CPPFormat2016::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	list=GetDlgItem(IDC_LIST_IDS);
	list.SetExtendedListViewStyle(
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString tmp;
	tmp.LoadString(IDS_COLHEAD_ENTITY);
	list.AddColumn(tmp,0);
	tmp.LoadString(IDS_COLHEAD_VALUE);
	list.AddColumn(tmp,1);
	RECT r;
	list.GetClientRect(&r);
	list.SetColumnWidth(0,(r.right-r.left)*70/100);
	list.SetColumnWidth(1,r.right-r.left-list.GetColumnWidth(0));
	CString dispName;
	CString ident;
	for(int i = 0; i < sizeof(entityData)/sizeof(entityData[0]); ++i){
		dispName.LoadString(entityData[i].id);
		list.InsertItem(i, dispName);
		ident = entityData[i].defval;
		try{
			ident = LPCTSTR(_bstr_t(vsu[entityData[i].regname]));
		}
		catch(hkey::regerror&){}
		catch(_com_error&){}
		list.SetItemText(i, 1, ident);
	}
	return TRUE;
}

LRESULT CPPEncoding::OnRadio(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	switch(wID){
	case IDC_E_CLASSIC:
		::ShowWindow(GetDlgItem(IDC_CHB_REPEAT_MODE), SW_HIDE);
		break;
	case IDC_E_MODE2016:
		::ShowWindow(GetDlgItem(IDC_CHB_REPEAT_MODE), SW_SHOW);
		break;
	default:
		break;
	}
	
	return 0;
}

int CPPEncoding::OnApply()
{
	if(DoDataExchange(true)){
		try{
			if(alterfunc==1){
				vsu.lvalue(_T("ErrMode"))=long(m_nErrMode);
			}
			else{
				vsu.lvalue(_T("Format"))=long(m_nFormat);
			}
			vsu.lvalue(_T("OEMInput"))=m_nInput==0?1L:0L;
			vsu.lvalue(_T("OEMOutput"))=m_nOutput==0?1L:0L;
			vsu.lvalue(_T("UseWatch"))=long(m_nUseWatch);
			vsu.lvalue(_T("RepeatMode"))=long(m_nRepeatMode);
			vsu.lvalue(_T("WatchDir"))=m_szWatchDir;
			vsu.lvalue(_T("WatchMask"))=m_szWatchMask;
			return PSNRET_NOERROR;
		}
		catch(hkey::regerror&){}
	}
	return PSNRET_INVALID;
}

int CPPDefConn::OnApply()
{
	if(DoDataExchange(true)){
		if(m_nDefConn && m_nProt==0){
			if(!chkln(m_nNumber)){
				CString tmp;
				tmp.LoadString(IDS_LOGNUM_ABSENT);
				CString title;
				title.LoadString(IDS_TITLE);
				MessageBox(tmp,title);
				::SetFocus(GetDlgItem(IDC_NUMBER));
				return PSNRET_INVALID;
			}
		}
		set_autorun(m_nAutoRun?true:false,vsuexepath);
		try{
			vsu.lvalue(_T("DefConn"))=long(m_nDefConn);
			vsu.lvalue(_T("dcProtocol"))=long(m_nProt);
			vsu.lvalue(_T("dcNumber"))=m_nNumber;
			vsu.lvalue(_T("dcPort"))=m_nPort;
			vsu.lvalue(_T("dcUser"))=m_nUser;
			vsu.lvalue(_T("dcPassword"))=m_nPassword;
			vsu.lvalue(_T("dcLocation"))=m_szLocation;
			return PSNRET_NOERROR;
		}
		catch(hkey::regerror&){}
	}
	return PSNRET_INVALID;
}

#include <tchar.h>

int CPPDeptCodes::OnApply()
{
	_variant_t varCode[10];
	for(long Dept=0;Dept<10;Dept++){
		TCHAR txtCode[32];
		list.GetItemText(Dept,1,txtCode,31);
		__int64&Code=varCode[Dept].cyVal.int64;
		Code=_ttoi64(txtCode);
		if(0<=Code && Code<=140737488355327){
		}
		else{
			list.SelectItem(Dept);
			return PSNRET_INVALID;
		}
	}
	for(Dept=0;Dept<10;Dept++){
		TCHAR txtDept[16];
		wsprintf(txtDept,TEXT("%lu"),Dept);
		CY&cyCode=varCode[Dept].cyVal;
		if(cyCode.Hi){
			TCHAR txtCode[32];
			list.GetItemText(Dept,1,txtCode,31);
			vsu_dept.lvalue(txtDept)=txtCode;
		}
		else{
			vsu_dept.lvalue(txtDept)=long(cyCode.Lo);
		}
	}
	return PSNRET_NOERROR;
}

int CPPFormat2016::OnApply()
{
	for(int i = 0; i < sizeof(entityData)/sizeof(entityData[0]); ++i){
		TCHAR txt[32];
		list.GetItemText(i, 1, txt, 31);
		vsu.lvalue(entityData[i].regname) = txt;
	}
	return PSNRET_NOERROR;
}

void CPPDefConn::assign(moveshow&dst,int show,int pos)
{
	dst.show=show;
	dst.pos=pos;
}

LRESULT CPPDefConn::OnRadio(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	switch(wID){
	case IDC_RADIO_LOGNUM:
		assign(statics[IDC_STATIC_LOGNUM-IDC_FIRST_STATIC],SW_SHOW,0);
		assign(statics[IDC_STATIC_NETNUM-IDC_FIRST_STATIC],SW_HIDE,0);
		assign(edits[IDC_NUMBER-IDC_FIRST_EDIT],SW_SHOW,0);
		assign(statics[IDC_STATIC_LOCATION-IDC_FIRST_STATIC],SW_SHOW,1);
		assign(edits[IDC_LOCATION-IDC_FIRST_EDIT],SW_SHOW,1);
		assign(statics[IDC_STATIC_PORT-IDC_FIRST_STATIC],SW_HIDE,0);
		assign(edits[IDC_PORT-IDC_FIRST_EDIT],SW_HIDE,0);
		break;
	case IDC_RADIO_NETNUM:
		assign(statics[IDC_STATIC_PORT-IDC_FIRST_STATIC],SW_SHOW,0);
		assign(edits[IDC_PORT-IDC_FIRST_EDIT],SW_SHOW,0);
		assign(statics[IDC_STATIC_LOGNUM-IDC_FIRST_STATIC],SW_HIDE,1);
		assign(statics[IDC_STATIC_NETNUM-IDC_FIRST_STATIC],SW_SHOW,1);
		assign(edits[IDC_NUMBER-IDC_FIRST_EDIT],SW_SHOW,1);
		assign(statics[IDC_STATIC_LOCATION-IDC_FIRST_STATIC],SW_SHOW,2);
		assign(edits[IDC_LOCATION-IDC_FIRST_EDIT],SW_SHOW,2);
		break;
	case IDC_RADIO_SIMPLE:
		assign(statics[IDC_STATIC_PORT-IDC_FIRST_STATIC],SW_SHOW,0);
		assign(edits[IDC_PORT-IDC_FIRST_EDIT],SW_SHOW,0);
		assign(statics[IDC_STATIC_LOCATION-IDC_FIRST_STATIC],SW_SHOW,1);
		assign(edits[IDC_LOCATION-IDC_FIRST_EDIT],SW_SHOW,1);
		assign(statics[IDC_STATIC_LOGNUM-IDC_FIRST_STATIC],SW_HIDE,2);
		assign(statics[IDC_STATIC_NETNUM-IDC_FIRST_STATIC],SW_HIDE,2);
		assign(edits[IDC_NUMBER-IDC_FIRST_EDIT],SW_HIDE,2);
		break;
	}

	domoveshow(
		IDC_FIRST_STATIC,
		sizeof(statics)/sizeof(statics[0]),
		statics,
		staticpos);
	domoveshow(
		IDC_FIRST_EDIT,
		sizeof(edits)/sizeof(edits[0]),
		edits,
		editpos);

	return 0;
}

void CPPDefConn::domoveshow(int item,int size,moveshow const*ctrls,POINT const*const pos)
{
	for(int i=0;i<size;i++,item++,ctrls++){
		POINT const*coord=pos+ctrls->pos;
		HWND hWnd=GetDlgItem(item);
		::ShowWindow(hWnd,ctrls->show);
		if(ctrls->show==SW_SHOW){
			::SetWindowPos(hWnd,NULL,coord->x,coord->y,0,0,SWP_NOZORDER | SWP_NOSIZE);
		}
	}
}

CMainDlg::CMainDlg():CPropertySheetImpl<CMainDlg>(IDS_TITLE)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	AddPage(m_pgDefConn);
	AddPage(m_pgEncoding);
	if(alterfunc==1){
		AddPage(m_pgFormat2016);
	}
	else{
		AddPage(m_pgDeptCodes);
	}
}

struct CPPDefConn::MinMax const CPPDefConn::MinMax[3]={
	{1L,65535L},
	{1L,30L},
	{0L,0L},
};

void CPPDeptCodes::OnListDblclk(CListViewCtrl & list,int minsi,int maxsi)
{
	iItem=-1;
	LVHITTESTINFO Info;
	memset(&Info,0,sizeof(Info));
	Info.iItem=Info.iSubItem=-1;
	GetCursorPos(&Info.pt);
	list.ScreenToClient(&Info.pt);
	list.SubItemHitTest(&Info);
	if(Info.iItem!=-1 && minsi<=Info.iSubItem && Info.iSubItem<=maxsi){
		iItem=Info.iItem;
		iSubItem=Info.iSubItem;
		list.EditLabel(iItem);
	}
}

LRESULT CPPDeptCodes::OnDblclkList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	OnListDblclk(list,1,1);
	return 0;
}

bool CPPDeptCodes::OnListBeginLabelEdit(CListViewCtrl & list)
{
	if(iItem==-1)
		return true;
	CEdit ce=list.GetEditControl();
	RECT r;
	list.GetSubItemRect(iItem,iSubItem,LVIR_LABEL,&r);
	//r.left+=4;
	ce.MoveWindow(&r);

	ce.SetMargins(4,4);
	ce.SetLimitText(15);//99.99
	digitsonlyOnChar.SubclassWindow(ce);

	_TCHAR buf[256];
	list.GetItemText(iItem,iSubItem,buf,sizeof(buf)/sizeof(*buf)-1);
	ce.SetWindowText(buf);
	return false;
}
LRESULT CPPDeptCodes::OnBeginLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return OnListBeginLabelEdit(list);
}

void CPPDeptCodes::OnListEndLabelEdit(CListViewCtrl & list,NMLVDISPINFO* pdi)
{
	if(pdi->item.pszText){
		list.SetItemText(iItem,iSubItem,pdi->item.pszText);
	}
	iItem=-1;
}

LRESULT CPPDeptCodes::OnEndLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	OnListEndLabelEdit(list,(NMLVDISPINFO*)pnmh);
	return 0;
}

LRESULT CPPEncoding::OnWatchDirChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFolderDialog fdlg;
	fdlg.DoModal();
	SetDlgItemText(IDC_WATCHDIR,fdlg.GetFolderPath());
	return 0;
}

void CPPFormat2016::OnListDblclk(CListViewCtrl & list,int minsi,int maxsi)
{
	iItem=-1;
	LVHITTESTINFO Info;
	memset(&Info,0,sizeof(Info));
	Info.iItem=Info.iSubItem=-1;
	GetCursorPos(&Info.pt);
	list.ScreenToClient(&Info.pt);
	list.SubItemHitTest(&Info);
	if(Info.iItem!=-1 && minsi<=Info.iSubItem && Info.iSubItem<=maxsi){
		iItem=Info.iItem;
		iSubItem=Info.iSubItem;
		list.EditLabel(iItem);
	}
}

LRESULT CPPFormat2016::OnDblclkList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	OnListDblclk(list,1,1);
	return 0;
}

bool CPPFormat2016::OnListBeginLabelEdit(CListViewCtrl & list)
{
	if(iItem==-1)
		return true;
	CEdit ce=list.GetEditControl();
	RECT r;
	list.GetSubItemRect(iItem,iSubItem,LVIR_LABEL,&r);
	//r.left+=4;
	ce.MoveWindow(&r);
	
	ce.SetMargins(4,4);
	if (iItem == 0) {
		// separator
		ce.SetLimitText(1);
		separatorOnChar.SubclassWindow(ce);
	}
	else {
		ce.SetLimitText(15);
		commandOnChar.SubclassWindow(ce);
	}
	
	_TCHAR buf[256];
	list.GetItemText(iItem,iSubItem,buf,sizeof(buf)/sizeof(*buf)-1);
	ce.SetWindowText(buf);
	return false;
}
LRESULT CPPFormat2016::OnBeginLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return OnListBeginLabelEdit(list);
}

void CPPFormat2016::OnListEndLabelEdit(CListViewCtrl & list,NMLVDISPINFO* pdi)
{
	if(pdi->item.pszText){
		list.SetItemText(iItem,iSubItem,pdi->item.pszText);
	}
	iItem=-1;
}

LRESULT CPPFormat2016::OnEndLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	OnListEndLabelEdit(list,(NMLVDISPINFO*)pnmh);
	return 0;
}

