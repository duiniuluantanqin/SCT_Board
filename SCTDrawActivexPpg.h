#if !defined(AFX_SCTDRAWACTIVEXPPG_H__BBA7AF3D_20FC_4E77_B946_05B75B1F807A__INCLUDED_)
#define AFX_SCTDRAWACTIVEXPPG_H__BBA7AF3D_20FC_4E77_B946_05B75B1F807A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCTDrawActivexPpg.h : Declaration of the CSCTDrawActivexPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSCTDrawActivexPropPage : See SCTDrawActivexPpg.cpp.cpp for implementation.

class CSCTDrawActivexPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSCTDrawActivexPropPage)
	DECLARE_OLECREATE_EX(CSCTDrawActivexPropPage)

// Constructor
public:
	CSCTDrawActivexPropPage();

// Dialog Data
	//{{AFX_DATA(CSCTDrawActivexPropPage)
	enum { IDD = IDD_PROPPAGE_SCTDRAWACTIVEX };
	int		m_Operations;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSCTDrawActivexPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCTDRAWACTIVEXPPG_H__BBA7AF3D_20FC_4E77_B946_05B75B1F807A__INCLUDED)
