#if !defined(AFX_TESTCLASS_H__6F0CEDCD_876A_4065_B41D_1E7BD9F3890F__INCLUDED_)
#define AFX_TESTCLASS_H__6F0CEDCD_876A_4065_B41D_1E7BD9F3890F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestClass window

class CTestClass : public CWnd
{
// Construction
public:
	CTestClass();
    int xy;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestClass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTestClass)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCLASS_H__6F0CEDCD_876A_4065_B41D_1E7BD9F3890F__INCLUDED_)
