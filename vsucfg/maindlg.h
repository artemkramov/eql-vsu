// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__AA52F064_806F_4A43_9648_85B5EFDC2D10__INCLUDED_)
#define AFX_MAINDLG_H__AA52F064_806F_4A43_9648_85B5EFDC2D10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define IDC_E_CLASSIC   IDC_F_VSU
#define IDC_E_MODE2016  IDC_F_ISL
#define MAX_EXCLUDE_CHARACTES 2000
#define DELIMITER_BYTE 0x43


#include <myreg.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlmisc.h>


#if (_ATL_VER < 0x0700)
#pragma message("_ATL_VER < 0x0700")
#else
#pragma message("_ATL_VER >= 0x0700")
#endif

template <typename classOnChar>
class CMoveEdit: 
	public CWindowImpl< CMoveEdit >,
	public classOnChar
{
	BEGIN_MSG_MAP( CNoNumEdit )
		MESSAGE_HANDLER( WM_CHAR, OnChar )
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()
	
	LRESULT OnWindowPosChanging( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		//_RPT0(_CRT_WARN,"OnWindowPosChanging\n");
		LPWINDOWPOS pwp=(LPWINDOWPOS)lParam;
		pwp->flags|=SWP_NOMOVE|SWP_NOSIZE;
		return 0;
	}
};

struct DigitsOnlyOnChar
{
	LRESULT OnChar( UINT, WPARAM wParam, LPARAM, BOOL& bHandled )
	{
		TCHAR ch = wParam;
		if( _T('0') <= ch && ch <= _T('9') || ch==8)
			bHandled = FALSE;
		else
			MessageBeep( 0 );
		return 0;
	}	
};

class CPPDefConn:
	public CPropertyPageImpl<CPPDefConn>,
	public CWinDataExchange<CPPDefConn>
{
	hkey vsu;
	// this is true if there is no DefConn setting in the registry at the moment of this class' instantiation
	bool setDefConn;
public:
	enum {IDD=IDD_DEFCONN};

	// Конструктор
  CPPDefConn();
  ~CPPDefConn();

	// Обработчики сообщений
	BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	LRESULT OnRadio(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	// Обработчики уведомлений страницы свойств
	int OnApply();

	// full path of installed vsu.exe
	CString vsuexepath;

	// DDX переменые
	int m_nAutoRun;	//auto start on windows startup
	int m_nDefConn;	//при загрузке программы сразу пытаться связаться с кассой
	int m_nProt;	//Выбранный протокол
	long m_nNumber;
	long m_nPort;
	long m_nUser;
	long m_nPassword;
	TCHAR m_szLocation[MAX_PATH];

	// Переменные для перемещения элементов управления
	POINT editpos[3];//Позиции элементов на момент загрузки
	POINT staticpos[3];
	struct moveshow{
		int show;//параметр для ShowWindow
		int pos;//индекс в массиве позиций
	};
	moveshow edits[3];
	moveshow statics[4];
	static void assign(moveshow&dst,int show,int pos);
	void domoveshow(int item,int last,moveshow const*,POINT const*const);

	struct MinMax
	{
		long Min;
		long Max;
	};
	static MinMax const MinMax[3];

  // Карты
	BEGIN_MSG_MAP_EX(CPPDefConn)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER(IDC_RADIO_LOGNUM, IDC_RADIO_SIMPLE, OnRadio)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPPDefConn>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPPDefConn)
		DDX_CHECK(IDC_CHB_AUTORUN, m_nAutoRun)
		DDX_CHECK(IDC_CHB_DEFCONN, m_nDefConn)
		DDX_RADIO(IDC_RADIO_LOGNUM, m_nProt)
		if(m_nProt!=2){
			DDX_UINT_RANGE(IDC_NUMBER, m_nNumber,MinMax[m_nProt].Min,MinMax[m_nProt].Max)
		}
		DDX_UINT_RANGE(IDC_PORT, m_nPort,1L,255L)
		DDX_UINT_RANGE(IDC_USER, m_nUser,0L,255L)
		DDX_UINT_RANGE(IDC_PASSWORD, m_nPassword,0L,999999L)
		DDX_TEXT(IDC_LOCATION, m_szLocation)
	
	END_DDX_MAP()

};

class CPPEncoding:
	public CPropertyPageImpl<CPPEncoding>,
	public CWinDataExchange<CPPEncoding>
{
	hkey vsu;
public:
	enum {IDD=IDD_ENCODING};

	// Конструктор
  CPPEncoding();
  ~CPPEncoding();

	// Обработчики сообщений
	BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
	LRESULT OnWatchDirChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRadio(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	// Обработчики уведомлений страницы свойств
	int OnApply();

	// DDX переменые
	int m_nErrMode;
	int m_nRepeatMode;
	int m_nFormat;
;	int m_nInput;
	int m_nOutput;
	int m_nUseWatch;
	TCHAR m_szWatchDir[MAX_PATH];
	TCHAR m_szWatchMask[MAX_PATH];

  // Карты
	BEGIN_MSG_MAP_EX(CPPEncoding)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDC_WATCHDIR_CHANGE,	OnWatchDirChange)
		COMMAND_RANGE_HANDLER(IDC_E_CLASSIC, IDC_E_MODE2016, OnRadio)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPPEncoding>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPPEncoding)
		if(alterfunc==1){
			DDX_RADIO(IDC_E_CLASSIC, m_nErrMode)
		}
		else{
			DDX_RADIO(IDC_F_VSU, m_nFormat)
		}
		DDX_RADIO(IDC_IN_OEM, m_nInput)
		DDX_RADIO(IDC_OUT_OEM, m_nOutput)
		DDX_CHECK(IDC_USE_WATCH, m_nUseWatch)
		DDX_CHECK(IDC_CHB_REPEAT_MODE, m_nRepeatMode)
		DDX_TEXT(IDC_WATCHDIR, m_szWatchDir)
		DDX_TEXT(IDC_WATCHMASK, m_szWatchMask)
	END_DDX_MAP()
};

class CPPExcludeCharacters:
	public CPropertyPageImpl<CPPExcludeCharacters>,
	public CWinDataExchange<CPPExcludeCharacters>
{
		hkey vsu;
	public:
		enum {IDD=IDD_EXCLUDE_CHAR};
		
		CPPExcludeCharacters();
		~CPPExcludeCharacters();
		
		BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
		
		int OnApply();
		
		TCHAR m_bExclude[MAX_PATH];
		
		BEGIN_MSG_MAP_EX(CPPExcludeCharacters)
			MSG_WM_INITDIALOG(OnInitDialog)
			CHAIN_MSG_MAP(CPropertyPageImpl<CPPExcludeCharacters>)
		END_MSG_MAP()

		BEGIN_DDX_MAP(CPPExcludeCharacters)
			DDX_TEXT(IDC_EXCLUDE, m_bExclude)
		END_DDX_MAP()

};

class CPPDeptCodes:public CPropertyPageImpl<CPPDeptCodes>
{
	hkey vsu_dept;
	CListViewCtrl list;
	CMoveEdit<DigitsOnlyOnChar> digitsonlyOnChar;
public:
	enum {IDD=IDD_DEPTCODES};

	// Конструктор
	CPPDeptCodes();
	~CPPDeptCodes();

	// Обработчики сообщений
	BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );

	// Обработчики уведомлений страницы свойств
	int OnApply();

	// Редактирование в списке
	int iItem;
	int iSubItem;
	void OnListDblclk(CListViewCtrl & list,int minsi,int maxsi);
	LRESULT OnDblclkList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	bool OnListBeginLabelEdit(CListViewCtrl & list);
	LRESULT OnBeginLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	void OnListEndLabelEdit(CListViewCtrl & list,NMLVDISPINFO* pdi);
	LRESULT OnEndLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  // Карты
	BEGIN_MSG_MAP_EX(CPPDeptCodes)
		MSG_WM_INITDIALOG(OnInitDialog)
		NOTIFY_HANDLER(IDC_LIST_DEPT, NM_DBLCLK, OnDblclkList)
		NOTIFY_HANDLER(IDC_LIST_DEPT, LVN_BEGINLABELEDIT, OnBeginLabelEditList)
		NOTIFY_HANDLER(IDC_LIST_DEPT, LVN_ENDLABELEDIT, OnEndLabelEditList)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPPDeptCodes>)
	END_MSG_MAP()
};

struct SeparatorOnChar
{
	LRESULT OnChar( UINT, WPARAM wParam, LPARAM, BOOL& bHandled )
	{
		TCHAR ch = wParam;
		switch (ch) {
		case _T('/'):
		case _T('@'):
		case _T(';'):
		case _T(':'):
		case _T('!'):
		case _T('|'):
		case _T('\\'):
			bHandled = FALSE;
			break;
		default:
			MessageBeep( 0 );
		}
		return 0;
	}	
};

struct CommandOnChar
{
	LRESULT OnChar( UINT, WPARAM wParam, LPARAM, BOOL& bHandled )
	{
		TCHAR ch = wParam;
		if( _T('A') <= ch && ch <= _T('Z') || ch==8)
			bHandled = FALSE;
		else
			MessageBeep( 0 );
		return 0;
	}	
};

class CPPFormat2016:public CPropertyPageImpl<CPPFormat2016>
{
	hkey vsu;
	CListViewCtrl list;
	CMoveEdit<SeparatorOnChar> separatorOnChar;
	CMoveEdit<CommandOnChar> commandOnChar;
public:
	enum {IDD=IDD_FORMAT2016};

	CPPFormat2016();
	~CPPFormat2016();

	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);

	int OnApply();

	int iItem;
	int iSubItem;
	void OnListDblclk(CListViewCtrl & list,int minsi,int maxsi);
	LRESULT OnDblclkList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	bool OnListBeginLabelEdit(CListViewCtrl & list);
	LRESULT OnBeginLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	void OnListEndLabelEdit(CListViewCtrl & list,NMLVDISPINFO* pdi);
	LRESULT OnEndLabelEditList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BEGIN_MSG_MAP_EX(CPPFormat2016)
		MSG_WM_INITDIALOG(OnInitDialog)
		NOTIFY_HANDLER(IDC_LIST_IDS, NM_DBLCLK, OnDblclkList)
		NOTIFY_HANDLER(IDC_LIST_IDS, LVN_BEGINLABELEDIT, OnBeginLabelEditList)
		NOTIFY_HANDLER(IDC_LIST_IDS, LVN_ENDLABELEDIT, OnEndLabelEditList)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPPFormat2016>)
	END_MSG_MAP()
};



//class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		//public CMessageFilter, public CIdleHandler
class CMainDlg : public CPropertySheetImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
//	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	CMainDlg();

	CPPDefConn m_pgDefConn;
	CPPEncoding m_pgEncoding;
	CPPDeptCodes m_pgDeptCodes;
	CPPFormat2016 m_pgFormat2016;
	CPPExcludeCharacters m_ExcludeCharacters;

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		//MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CPropertySheetImpl<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		return TRUE;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		hkey hk(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Help Co\\vsu"));
		hk.lvalue(_T("OEMInput"))=BST_CHECKED==IsDlgButtonChecked(IDC_IN_OEM)?1L:0L;
		hk.lvalue(_T("OEMOutput"))=BST_CHECKED==IsDlgButtonChecked(IDC_OUT_OEM)?1L:0L;
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__AA52F064_806F_4A43_9648_85B5EFDC2D10__INCLUDED_)
