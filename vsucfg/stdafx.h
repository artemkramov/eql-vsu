// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A871CD01_A95B_4D5A_8EE2_100070FF0A76__INCLUDED_)
#define AFX_STDAFX_H__A871CD01_A95B_4D5A_8EE2_100070FF0A76__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100
#define _WTL_NEW_PAGE_NOTIFY_HANDLERS


#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

// alterfunc is defined by ALTERFUNC macro 
// if ALTERFUNC is not defined then alterfunc is zero
// if ALTERFUNC is 1 then VSU behaves like is was required by customers in 2016
#if defined(ALTERFUNC)
const int alterfunc=ALTERFUNC;
#else
const int alterfunc=0;
#endif

#include <atlwin.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A871CD01_A95B_4D5A_8EE2_100070FF0A76__INCLUDED_)
