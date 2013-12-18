#if !defined(AFX_SCTDRAWACTIVEX_H__B047ED75_1BF6_496C_9D1D_D8C4A886FB07__INCLUDED_)
#define AFX_SCTDRAWACTIVEX_H__B047ED75_1BF6_496C_9D1D_D8C4A886FB07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCT Draw Activex.h : main header file for SCT DRAW ACTIVEX.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexApp : See SCT Draw Activex.cpp for implementation.

class CSCTDrawActivexApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCTDRAWACTIVEX_H__B047ED75_1BF6_496C_9D1D_D8C4A886FB07__INCLUDED)
