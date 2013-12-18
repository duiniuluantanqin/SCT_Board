#if !defined(AFX_STYLESDLG_H__B1845C7F_7BC2_424F_889E_D8CA2C8581E2__INCLUDED_)
#define AFX_STYLESDLG_H__B1845C7F_7BC2_424F_889E_D8CA2C8581E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StylesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStylesDlg : Property page dialog

class CStylesDlg : public COlePropertyPage
{
	DECLARE_DYNCREATE(CStylesDlg)
	DECLARE_OLECREATE_EX(CStylesDlg)

// Constructors
public:
	CStylesDlg();

// Dialog Data
	//{{AFX_DATA(CStylesDlg)
	enum { IDD = IDD_PROPPAGE_Styles };
	short	m_PenWidth;
	CString	m_FilePath;
	CString	m_temBmpPath;
	int	m_LineStyle;
	int	m_FillStyle;
	CString	m_bkBmp;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CStylesDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESDLG_H__B1845C7F_7BC2_424F_889E_D8CA2C8581E2__INCLUDED_)
