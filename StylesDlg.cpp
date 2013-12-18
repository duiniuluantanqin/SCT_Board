// StylesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCT Draw Activex.h"
#include "StylesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStylesDlg dialog

IMPLEMENT_DYNCREATE(CStylesDlg, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CStylesDlg, COlePropertyPage)
//{{AFX_MSG_MAP(CStylesDlg)
ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {80E0F9CB-C5CC-43BE-A474-45D96721A114}
IMPLEMENT_OLECREATE_EX(CStylesDlg, "SCT Draw Activex.CStylesDlg",
					   0x80e0f9cb, 0xc5cc, 0x43be, 0xa4, 0x74, 0x45, 0xd9, 0x67, 0x21, 0xa1, 0x14)
					   
					   
					   /////////////////////////////////////////////////////////////////////////////
					   // CStylesDlg::CStylesDlgFactory::UpdateRegistry -
					   // Adds or removes system registry entries for CStylesDlg
					   
					   BOOL CStylesDlg::CStylesDlgFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Define string resource for page type; replace '0' below with ID.
	
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
		m_clsid, IDS_STYLES_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CStylesDlg::CStylesDlg - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CStylesDlg::CStylesDlg() :
COlePropertyPage(IDD, IDS_STYLES_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CStylesDlg)
	m_PenWidth = 0;
	m_FilePath = _T("");
	m_temBmpPath = _T("");
	m_LineStyle =0;
	m_FillStyle =0;
	m_bkBmp = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CStylesDlg::DoDataExchange - Moves data between page and properties

void CStylesDlg::DoDataExchange(CDataExchange* pDX)
{
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CStylesDlg)
	DDP_Text(pDX, IDC_LineWidth, m_PenWidth, _T("PenWidth") );
	DDX_Text(pDX, IDC_LineWidth, m_PenWidth);
	//DDV_MinMaxInt(pDX, m_PenWidth, 1, 20);
	DDP_Text(pDX, IDC_FilePath, m_FilePath, _T("FilePath") );
	DDX_Text(pDX, IDC_FilePath, m_FilePath);
	DDP_Text(pDX, IDC_TemBmpPath, m_temBmpPath, _T("temBmpPath") );
	DDX_Text(pDX, IDC_TemBmpPath, m_temBmpPath);
	DDP_CBIndex(pDX, IDC_LineStyles, m_LineStyle, _T("LineStyle") );
	DDX_CBIndex(pDX, IDC_LineStyles, m_LineStyle);
	DDP_CBIndex(pDX, IDC_FillStyles, m_FillStyle, _T("FillStyle") );
	DDX_CBIndex(pDX, IDC_FillStyles, m_FillStyle);
	DDP_Text(pDX, IDC_bkImage, m_bkBmp, _T("bkBmp") );
	DDX_Text(pDX, IDC_bkImage, m_bkBmp);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}
/////////////////////////////////////////////////////////////////////////////
// CStylesDlg message handlers

void CStylesDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(false,"sct",NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,".sct|*.sct|",this);
	if(FileDlg.DoModal()==IDOK)
	{
		m_FilePath=FileDlg.GetPathName();	 
		SetDlgItemText(IDC_FilePath,m_FilePath);
	}
}

void CStylesDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
    char Buffer[MAX_PATH];
    bi.hwndOwner = NULL;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = Buffer;
    bi.lpszTitle="ÔÝ´æÍ¼Æ¬Â·¾¶";
    bi.ulFlags=0;
    bi.lpfn = NULL;
	// bi.iImage=IDR_MAINFRAME;
    LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
    if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		m_temBmpPath=Buffer;
		SetDlgItemText(IDC_TemBmpPath,m_temBmpPath);
    }
}

void CStylesDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
   	CFileDialog FileDlg(true,"bmp",NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,".bmp;.jpg|*.bmp;*.jpg|.EMF;.EMF+;WMF|*.emf;*.emf+;*.wmf|",this);
	if(FileDlg.DoModal()==IDOK)
	{
		m_bkBmp=FileDlg.GetPathName();
		SetDlgItemText(IDC_bkImage,m_bkBmp);
	}
	else
	{
		m_bkBmp="";
		SetDlgItemText(IDC_bkImage,m_bkBmp);
	}
}
